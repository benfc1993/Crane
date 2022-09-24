#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/Shader/OpenGLShader.h"

#include "Panels/ParticleSystemPropertiesPanel.h"
#include "Panels/RenderStatsPanel.h"

namespace Crane {

    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1.6f / 0.9f), m_ParticleSystem(10000) {}

    void EditorLayer::OnAttach()
    {
        m_Texture = Crane::Texture2D::Create("assets/textures/logo.png");

        Crane::FramebufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        m_Framebuffer = Crane::Framebuffer::Create(spec);

        m_Particle.ColorBegin = { 0.8f, 0.2f, 0.3f, 1.0f };
        m_Particle.ColorEnd = { 0.036f, 0.044f, 0.054f, 0.059f };
        m_Particle.Lifetime = 9.5f;
        m_Particle.LifetimeVariation = 0.6f;
        m_Particle.Position = { 0.0f, 0.0f };
        m_Particle.SizeBegin = 0.2f;
        m_Particle.SizeEnd = 0.75f;
        m_Particle.SizeVariation = 0.7f;
        m_Particle.Velocity = { 0.420f, 0.370f };
        m_Particle.VelocityVariation = { 1.0f, 0.4f };
        m_Particle.Texture = m_Texture;
    }
    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Crane::Time time)
    {
        CR_PROFILE_FUNCTION();

        m_CameraController.OnUpdate(time);

        Crane::Renderer2D::ResetStats();

        {
            CR_PROFILE_SCOPE("Renderer Prep");

            m_Framebuffer->Bind();

            Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
            Crane::RenderCommand::Clear();
        }

        {
            CR_PROFILE_SCOPE("Renderer Draw");
            Crane::TextureParameters textureParameters(m_Texture);
            textureParameters.Color = m_Color;

            Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());
            Crane::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_Color);
            Crane::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 0.5f, 0.75f }, textureParameters);
            Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, Crane::TextureParameters(m_Texture));
            Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, Crane::TextureParameters(m_Texture));
            Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, Crane::TextureParameters(m_Texture));
            Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, Crane::TextureParameters(m_Texture));
            Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, Crane::TextureParameters(m_Texture));

            for (int i = 0; i < m_Particle.BurstSize; i++)
            {
                m_ParticleSystem.Emit(m_Particle);
            }

            m_ParticleSystem.OnUpdate(time);
            m_ParticleSystem.OnRender(m_CameraController.GetCamera());
        }

        Crane::Renderer2D::EndScene();

        m_Framebuffer->Unbind();

    }
    void EditorLayer::OnImGuiRender()
    {
        CR_PROFILE_FUNCTION();


        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        bool p_open = true;

        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);


        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Exit")) Crane::Application::Get().Close();
                ImGui::Separator();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        ImGui::Begin("Settings");
        ImGui::ColorEdit4("Triangle color", glm::value_ptr(m_Color));
        ImGui::DragFloat3("Position", glm::value_ptr(m_Position));
        ImGui::SliderAngle("Rotation", &m_Angle);
        ImGui::DragFloat2("Scale", glm::value_ptr(m_Scale));
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (m_ViewportSize != *((glm::vec2*)&viewportSize))
        {
            m_ViewportSize = { viewportSize.x, viewportSize.y };
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

            m_CameraController.OnResize(viewportSize.x, viewportSize.y);
        }
        uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererId();
        ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ParticleSystemPropertiesPanel(m_Particle);

        RenderStatsPanel();
        ImGui::End();
    }
    void EditorLayer::OnEvent(Crane::Event& event)
    {
        m_CameraController.OnEvent(event);

    }
}