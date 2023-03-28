#include "EditorViewport.h"

#include "Crane/Renderer/Renderer2D.h"
#include "Crane/Renderer/RenderCommand.h"
#include "Crane/Scene/Components.h"
#include "Crane/Core/Application.h"
#include "Crane/Input/Input.h"
#include "Crane/Math/Math.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

namespace Crane {

	EditorViewport::EditorViewport(std::string name, Ref<Scene> scene, EditorLayer* editorLayer): Viewport(name, scene)
	{
		m_editorLayer = editorLayer;
		m_camera = EditorCamera();
		m_imGuizmoLayer = CreateRef<ImGuizmoLayer>(m_camera, m_Scene);

		FramebufferSpecification spec;
		spec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
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

		auto sceneState = m_Scene->GetState();

		switch (sceneState)
		{
		case SceneState::Edit:
			m_Scene->OnUpdateEditor(time, m_camera);
			break;
		case SceneState::Play:
			m_Scene->OnUpdateRuntime(time);
			break;
		case SceneState::Simulate:
			m_Scene->OnUpdateSimulation(time, m_camera);
			break;
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_Scene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void EditorViewport::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_Name.c_str());



		auto viewportOffset = ImGui::GetWindowPos();
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
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
		// CR_CORE_INFO("Image: {} - {}", m_ViewportSize.x, m_ViewportSize.y);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE"))
			{
				const std::filesystem::path path = std::filesystem::path((char*)payload->Data);

				// m_EditorLayer::LoadScene(std::filesystem::path(s_AssetPath) / path);
				ImGui::EndDragDropTarget();
			}
		}

		//Disable alt to togle nav
		if (ImGui::IsKeyDown(Key::LeftAlt) || ImGui::IsKeyDown(Key::RightAlt))
		{
			ImGui::GetCurrentContext()->NavWindowingToggleLayer = false;
		}

		//Gizmos
		Entity selectedEntity = GetSelectedEntity();

		m_CanPick = m_ViewportHovered;

		if (selectedEntity)
		{

			bool usingGuizmo = m_GizmoType != -1 && (ImGuizmo::IsUsing() || ImGuizmo::IsOver());
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				// Edit camera
				const glm::mat4& cameraProjection = m_camera.GetProjection();
				const glm::mat4& cameraView = m_camera.GetViewMatrix();

				//Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.Transform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float translationSnapValue = 0.1f;
				float rotationSnapValue = 45.0f;
				float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? rotationSnapValue : translationSnapValue;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);



				if (usingGuizmo)
				{

					glm::vec3 parentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 parentScale = glm::vec3(1.0f, 1.0f, 1.0f);

					auto& hc = selectedEntity.GetComponent<HierarchyComponent>();

					if (hc.Parent != 0)
					{
						Entity parent = m_Scene->GetEntityByUUID(hc.Parent);
						auto& parentTransform = parent.GetComponent<TransformComponent>();
						parentPosition = parentTransform.WorldPosition;
						parentScale = parentTransform.WorldScale;
					}

					glm::vec3 position, rotation, scale;
					Math::DecomposeTransform(transform, position, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Position = position - parentPosition;
					tc.Rotation += deltaRotation;
					tc.Scale = scale / parentScale;
				}
				m_CanPick = m_ViewportHovered && !usingGuizmo;



				// bool usingGuizmo = m_imGuizmoLayer->OnUpdate(selectedEntity, m_ViewportBounds[0], m_ViewportBounds[1]);
				// m_CanPick = m_ViewportHovered && !usingGuizmo;
			}
		}

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

	void EditorViewport::OnEvent(Event& event)
	{
		auto dispatcher = EventDispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(HandleInput));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CR_BIND_EVENT_FN(OnMouseButtonPressed));
		m_camera.OnEvent(event);
	}

	bool EditorViewport::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (!m_CanPick || Input::IsKeyPressed(Key::LeftAlt)) return false;
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
		case Key::Q:
			m_GizmoType = -1;
			return true;
		case Key::W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			return true;
		case Key::E:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			return true;
		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			return true;

		}
		return false;
	}
}