#include "RuntimeViewport.h"

#include "Crane/Renderer/Renderer2D.h"
#include "Crane/Renderer/RenderCommand.h"
#include "Crane/Core/Application.h"

#include <imgui/imgui.h>

namespace Crane {
	RuntimeViewport::RuntimeViewport(std::string name, Ref<Scene> scene): Viewport(name, scene)
	{
		FramebufferSpecification spec;
		spec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		spec.Width = 1280;
		spec.Height = 720;
		m_Framebuffer = Framebuffer::Create(spec);
	}

	void RuntimeViewport::OnUpdate(Time time)
	{
		CR_PROFILE_FUNCTION();

		m_PrimaryCamera = m_ActiveScene->GetPrimaryCameraEntity();


		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();

		{
			CR_PROFILE_SCOPE("Renderer Prep");

			m_Framebuffer->Bind();

			RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
			RenderCommand::Clear();

			m_Framebuffer->ClearAttachment(1, -1);
		}

		Camera* camera = &m_PrimaryCamera.GetComponent<CameraComponent>().Camera;
		TransformComponent* cameraTransform = &m_PrimaryCamera.GetComponent<TransformComponent>();

		m_ActiveScene->Render(time, camera, cameraTransform->Transform());

		m_Framebuffer->Unbind();
	}
	void RuntimeViewport::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_Name.c_str());

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		Application::Get().GetImGuiLayer()->ShouldBlockEvents(!m_ViewportHovered);

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportSize.x, viewportSize.y };

		uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
		ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}
	void RuntimeViewport::OnSceneStateChanged(Ref<Scene> scene)
	{
		m_Scene = scene;
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}
}