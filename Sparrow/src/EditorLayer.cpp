#include "EditorLayer.h"
// #include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/Shader/OpenGLShader.h"

#include "Panels/ParticleSystemPropertiesPanel.h"
#include "Panels/RenderStatsPanel.h"

#include "Crane/Scene/ScriptableEntity.h"

namespace Crane
{

    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1.6f / 0.9f) {}

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        m_Framebuffer = Framebuffer::Create(spec);

        m_ActiveScene = CreateRef<Scene>();

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>();

        m_SecondCameraEntity = m_ActiveScene->CreateEntity("Second Camera");
        m_SecondCameraEntity.AddComponent<CameraComponent>().Primary = false;

        class CameraController : public ScriptableEntity
        {
        public:
            virtual void OnCreate()
            {

            }

            void OnDestroy()
            {

            }

            void OnUpdate(Time time)
            {
                auto& transform = GetComponent<TransformComponent>();
                float moveSpeed = 5.0f;
                float rotationSpeed = 50.0f;

                if (Input::IsKeyPressed(KeyCode::A))
                    transform.Position.x -= moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(KeyCode::D))
                    transform.Position.x += moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(KeyCode::S))
                    transform.Position.y -= moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(KeyCode::W))
                    transform.Position.y += moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(KeyCode::Q))
                    transform.Rotation.z -= rotationSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(KeyCode::E))
                    transform.Rotation.z += rotationSpeed * time.DeltaTime();
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        m_HierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Time time)
    {
        CR_PROFILE_FUNCTION();

        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        if (m_ViewportFocused)
            m_CameraController.OnUpdate(time);

        Renderer2D::ResetStats();

        {
            CR_PROFILE_SCOPE("Renderer Prep");

            m_Framebuffer->Bind();

            RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
            RenderCommand::Clear();
        }

        m_ActiveScene->OnUpdate(time);

        m_Framebuffer->Unbind();
    }
    void EditorLayer::OnImGuiRender()
    {
        CR_PROFILE_FUNCTION();

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

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
        ImGuiStyle& style = ImGui::GetStyle();

        float minWindowSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("DarkMode"))
                    Application::Get().GetImGuiLayer()->SetDarkThemeColors();
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::Separator();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        style.WindowMinSize.x = minWindowSize;

        ImGui::Begin("Settings");

        ImGui::Separator();

        if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
        {
            m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
            m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
        }

        ImGui::Separator();

        ImGui::End();

        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();

            Application::Get().GetImGuiLayer()->ShouldBlockEvents(!m_ViewportHovered || !m_ViewportFocused);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            m_ViewportSize = { viewportSize.x, viewportSize.y };

            uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
            ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            ImGui::End();
            ImGui::PopStyleVar();
        }

        m_RenderStatsPanel.OnImGuiRender();

        m_HierarchyPanel.OnImGuiRender();

        ImGui::Begin("Theme editor");
        ImGuiLayer::ThemeColors& theme = Application::Get().GetImGuiLayer()->m_Theme;
        if (ImGui::ColorEdit4("Primary", (float*)&theme.Primary) ||
            ImGui::ColorEdit4("Sceondary", (float*)&theme.Secondary) ||
            ImGui::ColorEdit4("White", (float*)&theme.White) ||
            ImGui::ColorEdit4("Dark", (float*)&theme.Dark) ||
            ImGui::ColorEdit4("Medium", (float*)&theme.Medium) ||
            ImGui::ColorEdit4("Light", (float*)&theme.Light) ||
            ImGui::ColorEdit4("WindowBg", (float*)&theme.WindowBg))
            Application::Get().GetImGuiLayer()->SetDarkThemeColors();
        ImGui::End(); // Theme Editor

        ImGui::End();
    }
    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }
}