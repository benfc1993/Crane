#include "EditorLayer.h"
// #include "imgui/imgui.h"
#include "Crane/Scene/ScriptableEntity.h"
#include "Crane/Utils/PlatformUtils.h"
#include "Platform/OpenGL/Shader/OpenGLShader.h"
#include "Crane/Math/Math.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <sys/stat.h>
#include <ImGuizmo.h>

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

        Entity particlesDefault = m_ActiveScene->CreateEntity("Particles default");
        particlesDefault.AddComponent<ParticleSystemComponent>();

        Entity particlesTexture = m_ActiveScene->CreateEntity("Particles texture");
        particlesTexture.AddComponent<ParticleSystemComponent>(10000, "assets/textures/white-smoke.png");

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

                if (Input::IsKeyPressed(Key::A))
                    transform.Position.x -= moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(Key::D))
                    transform.Position.x += moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(Key::S))
                    transform.Position.y -= moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(Key::W))
                    transform.Position.y += moveSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(Key::Q))
                    transform.Rotation.z -= rotationSpeed * time.DeltaTime();
                if (Input::IsKeyPressed(Key::E))
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
                if (ImGui::MenuItem("Preferences"))
                    m_SettingsPanel.OpenPanel();

                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                {
                    SaveScene();
                }

                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                {
                    SaveSceneAs();
                }

                if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                {
                    OpenScene();
                }
                if (ImGui::MenuItem("Exit"))
                    Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        style.WindowMinSize.x = minWindowSize;


        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            bool imGuiInteracting = ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused();

            Application::Get().GetImGuiLayer()->ShouldBlockEvents((!m_ViewportFocused || !m_ViewportHovered) && imGuiInteracting);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            m_ViewportSize = { viewportSize.x, viewportSize.y };

            uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
            ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            //Gizmos
            Entity selectedEntity = m_HierarchyPanel.GetSelectedEntity();
            if (selectedEntity && m_GizmoType != -1)
            {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();
                float windowWidth = ImGui::GetWindowWidth();
                float windowHeight = ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

                //Camera
                auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                const auto& camera = cameraEntity.GetComponent<CameraComponent>();
                const glm::mat4& cameraProjection = camera.Camera.GetProjection();
                glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().Transform);

                //Entity transform
                auto& tc = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = tc.Transform;

                // Snapping
                bool snap = Input::IsKeyPressed(Key::LeftControl);
                float translationSnapValue = 0.1f;
                float rotationSnapValue = 45.0f;
                float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? rotationSnapValue : translationSnapValue;

                float snapValues[3] = { snapValue, snapValue, snapValue };

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                    (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

                if (ImGuizmo::IsUsing())
                {
                    glm::vec3 position, rotation, scale;
                    Math::DecomposeTransform(transform, position, rotation, scale);

                    glm::vec3 deltaRotation = rotation - tc.Rotation;
                    tc.Position = position;
                    tc.Rotation += deltaRotation;
                    tc.Scale = scale;
                }
            }

            ImGui::End();
            ImGui::PopStyleVar();
        }

        m_RenderStatsPanel.OnImGuiRender();

        m_HierarchyPanel.OnImGuiRender();
        m_SettingsPanel.OnImGuiRender();

        ImGui::End();
    }
    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        m_CameraController.OnEvent(event);
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
        CR_INFO("Press");
        switch (e.GetKeyCode())
        {
        case Key::N:
            if (control)
            {
                NewScene();
                return true;
            }
        case Key::O:
            if (control)
            {
                OpenScene();
                return true;
            }
        case Key::S:
            if (control)
            {
                SaveScene();
                return true;
            }
            if (control && shift)
            {
                SaveSceneAs();
                return true;
            }
            //Gizmo shortcuts

        case Key::Q:
            m_GizmoType = -1;
            break;
        case Key::W:
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        case Key::E:
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        case Key::R:
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        default:
            return false;
        }

    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_HierarchyPanel.SetContext(m_ActiveScene);
    }
    void EditorLayer::OpenScene()
    {
        std::string filePath = FileDialogs::OpenFile();
        if (!filePath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_HierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filePath);
        }
    }
    void EditorLayer::SaveScene()
    {
        struct stat buffer;
        int fileExists = stat(m_ActiveScene->GetFilePath().c_str(), &buffer);

        if (fileExists == 0)
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_ActiveScene->GetFilePath());
        }
        else
        {
            SaveSceneAs();
        }
    }
    void EditorLayer::SaveSceneAs()
    {
        std::string filePath = FileDialogs::SaveFile();
        if (!filePath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filePath);
            m_ActiveScene->SetFilePath(filePath);
        }
    }
}