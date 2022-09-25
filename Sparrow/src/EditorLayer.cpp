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
        m_Texture = Texture2D::Create("assets/textures/logo.png");

        FramebufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        m_Framebuffer = Framebuffer::Create(spec);

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
        m_Particle.Texture = Texture2D::Create("assets/textures/white-smoke.png");

        m_ActiveScene = CreateRef<Scene>();

        m_QuadEntity = m_ActiveScene->CreateEntity("Square");
        m_QuadEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.2f, 0.3f, 0.8f, 1.0f });
    }
    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnUpdate(Time time)
    {
        CR_PROFILE_FUNCTION();
        if (m_ViewportFocused)
            m_CameraController.OnUpdate(time);


        Renderer2D::ResetStats();

        {
            CR_PROFILE_SCOPE("Renderer Prep");

            m_Framebuffer->Bind();

            RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
            RenderCommand::Clear();
        }


        Renderer2D::BeginScene(m_CameraController.GetCamera());
        {
            CR_PROFILE_SCOPE("Renderer Draw");

            for (int i = 0; i < m_Particle.BurstSize; i++)
            {
                m_ParticleSystem.Emit(m_Particle);
            }

            m_ParticleSystem.OnUpdate(time);
            m_ParticleSystem.OnRender(m_CameraController.GetCamera());
        }
        m_ActiveScene->OnUpdate(time);
        Renderer2D::EndScene();

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
                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::Separator();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        auto& tag = m_QuadEntity.GetComponent<TagComponent>().Tag;
        auto& transform = m_QuadEntity.GetComponent<TransformComponent>();
        auto& sprite = m_QuadEntity.GetComponent<SpriteRendererComponent>().Color;

        ImGui::Begin("Settings");
        ImGui::Separator();
        ImGui::Text("%s", tag.c_str());
        ImGui::ColorEdit4("Triangle color", glm::value_ptr(sprite));
        ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
        ImGui::SliderAngle("Rotation", &transform.Rotation);
        ImGui::DragFloat2("Scale", glm::value_ptr(transform.Scale), 0.1f);
        ImGui::Separator();
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();

        Application::Get().GetImGuiLayer()->ShouldBlockEvents(!m_ViewportHovered || !m_ViewportFocused);

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
    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);

    }
}