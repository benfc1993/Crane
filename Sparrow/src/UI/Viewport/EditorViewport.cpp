#include "EditorViewport.h"

#include "Crane/Renderer/Renderer2D.h"
#include "Crane/Renderer/RenderCommand.h"
#include "Crane/Scene/Components.h"
#include "Crane/Core/Application.h"
#include "Crane/Input/Input.h"
#include "Crane/Math/Math.h"

#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Crane {

	EditorViewport::EditorViewport(std::string name, Ref<Scene> scene, EditorLayer* editorLayer): Viewport(name, scene), m_imGuizmoLayer(ImGuizmoLayer(&m_camera, m_Scene))
	{
		m_editorLayer = editorLayer;
		m_camera = EditorCamera();


		FramebufferSpecification spec;
		spec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
	}

	EditorViewport::~EditorViewport()
	{
		ImGui::DestroyContext();
	}

	glm::vec2 EditorViewport::GetMousePosition()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		return { mouseX, mouseY };
	}

	void EditorViewport::OnUpdate(Time time)
	{
		CR_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			// CR_CORE_INFO("Update: {} - {}", m_ViewportSize.x, m_ViewportSize.y);
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_Scene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		if (m_ViewportFocused)
			m_camera.OnUpdate(time);

		Renderer2D::ResetStats();

		{
			CR_PROFILE_SCOPE("Renderer Prep");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
			RenderCommand::Clear();

			m_Framebuffer->ClearAttachment(1, -1);
		}

		m_ActiveScene->Render(time, m_camera);

		auto mousePos = GetMousePosition();
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];


		if (m_ViewportHovered && mousePos.x >= 0 && mousePos.y >= 0 && mousePos.x < (int)viewportSize.x && mousePos.y < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mousePos.x, mousePos.y);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_Scene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorViewport::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_Name.c_str(), 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar);

		auto viewportOffset = ImGui::GetWindowPos();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		bool imGuiInteracting = ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused();

		if (m_ViewportFocused && m_editorLayer->GetPanels()->GetActiveScene() != m_Scene)
		{
			m_editorLayer->GetPanels()->SetActiveScene(m_Scene);
		}

		Application::Get().GetImGuiLayer()->ShouldBlockEvents(!m_ViewportHovered || imGuiInteracting);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportSize.x, viewportSize.y };

		uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		m_ViewportHovered = ImGui::IsItemHovered();

		// CR_CORE_INFO("Image: {} - {}", m_ViewportSize.x, m_ViewportSize.y);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE"))
			{
				const std::filesystem::path path = std::filesystem::path((char*)payload->Data);

				auto extension = path.extension();

				if (extension == ".scene")
				{
					m_editorLayer->LoadScene(path);
				}

				if (extension == ".prefab")
				{
					auto prefab = PrefabSerialiser::DeserialisePrefab(path, m_ActiveScene, true);
					if (prefab)
						m_editorLayer->GetPanels()->SetSelectedEntity(prefab);
				}

				ImGui::EndDragDropTarget();
			}
		}

		//Disable alt to togle nav
		if (ImGui::IsKeyDown(Key::LeftAlt) || ImGui::IsKeyDown(Key::RightAlt))
		{
			ImGui::GetCurrentContext()->NavWindowingToggleLayer = false;
		}


		m_CanPick = m_ViewportHovered;

		Entity selectedEntity = GetSelectedEntity();

		DrawImGuizmo(selectedEntity);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EditorViewport::OnOverlayRender()
	{
		glm::vec4 selectedColor = glm::vec4(1, 0.5f, 0, 1);
		glm::vec4 colliderColor = glm::vec4(0, 1, 0, 1);


		Renderer2D::BeginScene(m_camera);


		Renderer2D::SetLineWidth(3.0f);

		float zIndex = 0.001f;
		glm::vec3 cameraForwardDirection = m_camera.GetForwardDirection();
		glm::vec3 projectionCollider = cameraForwardDirection * glm::vec3(zIndex);

		auto selectedEntity = GetSelectedEntity();
		if (selectedEntity)
		{
			auto& transform = selectedEntity.GetComponent<TransformComponent>();
			if (selectedEntity.HasComponent<SpriteRendererComponent>())
			{

				Renderer2D::DrawRect(glm::translate(transform.Transform(), glm::vec3(0.0f, 0.0f, -projectionCollider.z)), selectedColor);
			}

			if (selectedEntity.HasComponent<CircleRendererComponent>())
			{
				Renderer2D::DrawCircle(glm::translate(transform.Transform(), glm::vec3(0.0f, 0.0f, -projectionCollider.z)), selectedColor, 0.075f);
			}
		}

		if (m_editorLayer->GetSettings()->Get()->ShowColiders)
		{

			{
				auto view = m_Scene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();

				for (auto entity : view)
				{
					auto [transformC, box] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 position = transformC.WorldPosition + glm::vec3(box.Offset, -projectionCollider.z);
					glm::vec3 scale = transformC.Scale * glm::vec3(box.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), transformC.WorldPosition)
						* glm::rotate(glm::mat4(1.0f), transformC.Rotation.z, glm::vec3(0, 0, 1))
						* glm::translate(glm::mat4(1.0f), glm::vec3(box.Offset, -projectionCollider.z))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, colliderColor);
				}
			}

			{
				auto view = m_Scene->GetAllEntitiesWith<TransformComponent, CircleColliderComponent>();

				for (auto entity : view)
				{
					auto [transformC, circle] = view.get<TransformComponent, CircleColliderComponent>(entity);

					glm::vec3 scale = transformC.Scale * glm::vec3(circle.Radius * 2.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), transformC.Position)
						* glm::rotate(glm::mat4(1.0f), transformC.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(circle.Offset, -projectionCollider.z))
						* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.x, scale.z));

					Renderer2D::DrawCircle(transform, colliderColor, 0.05f);
				}
			}

		}

		Renderer2D::EndScene();

		Renderer2D::SetLineWidth(2.0f);

	}

	void EditorViewport::DrawImGuizmo(Entity selectedEntity)
	{
		auto usingGuizmo = m_imGuizmoLayer.OnUpdate(selectedEntity, m_ViewportBounds);
		m_CanPick &= !usingGuizmo;
	}

	void EditorViewport::OnSceneStateChanged(Ref<Scene> scene)
	{
		m_Scene = scene;
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}


	void EditorViewport::OnEvent(Event& event)
	{
		auto dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(HandleInput));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CR_BIND_EVENT_FN(OnMouseButtonPressed));
		m_camera.OnEvent(event);
	}

	bool EditorViewport::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (!m_CanPick || !m_ViewportHovered || Input::IsKeyPressed(Key::LeftAlt)) return false;
		if (e.getMouseButton() == 0)
		{
			m_editorLayer->GetPanels()->SetSelectedEntity(m_HoveredEntity);
		}
		return true;
	}

	bool EditorViewport::HandleInput(KeyPressedEvent& event)
	{
		switch (event.GetKeyCode())
		{
		}
		return false;
	}
}