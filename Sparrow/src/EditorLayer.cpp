#include "EditorLayer.h"
#include "imgui/imgui_demo.cpp"
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
    extern const std::filesystem::path s_AssetPath = "assets";

    EditorLayer::EditorLayer() : Layer("EditorLayer") {}

    void EditorLayer::OnAttach()
    {
        FramebufferSpecification spec;
        spec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
        spec.Width = 1280;
        spec.Height = 720;
        m_Framebuffer = Framebuffer::Create(spec);

        m_ActiveScene = CreateRef<Scene>();
        m_Panels.SetActiveScene(m_ActiveScene);

        m_Panels.AddPanel<SceneHierarchyPanel>(m_ActiveScene, true);

        m_SettingsPanel = CreateRef<SettingsPanel>(true);
        m_Panels.AddPanel(m_SettingsPanel);

        m_Panels.AddPanel<RenderStatsPanel>();

        m_Panels.AddPanel<ContentBrowserPanel>(true);

        m_Panels.AddPanel<SceneToolbar>(std::bind(&EditorLayer::OnScenePlay, this), std::bind(&EditorLayer::OnSceneStop, this));
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
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        if (m_ViewportFocused)
            m_EditorCamera.OnUpdate(time);

        Renderer2D::ResetStats();

        {
            CR_PROFILE_SCOPE("Renderer Prep");

            m_Framebuffer->Bind();

            RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
            RenderCommand::Clear();

            m_Framebuffer->ClearAttachment(1, -1);
        }

        auto sceneState = m_ActiveScene->GetState();

        switch (m_SceneState)
        {
        case SceneState::Edit:
            m_ActiveScene->OnUpdateEditor(time, m_EditorCamera);
            break;
        case SceneState::Play:
            m_ActiveScene->OnUpdateRuntime(time);
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
            m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }


        m_Framebuffer->Unbind();
    }
    void EditorLayer::OnImGuiRender()
    {
        CR_PROFILE_FUNCTION();


        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
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



        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();
        ImGui::ShowDemoWindow();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();

        float minWindowSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        style.WindowMinSize.x = minWindowSize;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Preferences"))
                    m_SettingsPanel->OpenPanel();

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

            if (ImGui::BeginMenu("Panels"))
            {
                if (ImGui::MenuItem("Add Hierarchy Panel"))
                    m_Panels.AddPanel<SceneHierarchyPanel>(m_ActiveScene);

                if (ImGui::MenuItem("Add RenderStats Panel"))
                    m_Panels.AddPanel<RenderStatsPanel>();
                if (ImGui::MenuItem("Add Content Browser Panel"))
                    m_Panels.AddPanel<ContentBrowserPanel>();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }


        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Viewport");



            auto viewportOffset = ImGui::GetWindowPos();
            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            m_ViewportFocused = ImGui::IsWindowFocused();
            m_ViewportHovered = ImGui::IsWindowHovered();
            bool imGuiInteracting = ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused();

            Application::Get().GetImGuiLayer()->ShouldBlockEvents((!m_ViewportFocused || !m_ViewportHovered) && imGuiInteracting);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            m_ViewportSize = { viewportSize.x, viewportSize.y };

            uint64_t textureId = m_Framebuffer->GetColorAttachmentRendererId();
            ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_FILE"))
                {
                    const std::filesystem::path path = std::filesystem::path((char*)payload->Data);

                    LoadScene(std::filesystem::path(s_AssetPath) / path);
                    ImGui::EndDragDropTarget();
                }
            }

            //Disable alt to togle nav
            if (ImGui::IsKeyDown(Key::LeftAlt) || ImGui::IsKeyDown(Key::RightAlt))
            {
                ImGui::GetCurrentContext()->NavWindowingToggleLayer = false;
            }


            //Gizmos
            // if (m_ActiveScene->GetState() == SceneState::Edit)
            if (m_SceneState == SceneState::Edit)
            {
                Entity selectedEntity = m_Panels.GetSelectedEntity();
                if (selectedEntity && m_GizmoType != -1)
                {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();
                    float windowWidth = ImGui::GetWindowWidth();
                    float windowHeight = ImGui::GetWindowHeight();
                    ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

                    // Edit Camera
                    const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                    const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();

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

                    if (ImGuizmo::IsUsing())
                    {
                        m_CanPick = false;
                        glm::vec3 position, rotation, scale;
                        Math::DecomposeTransform(transform, position, rotation, scale);

                        glm::vec3 deltaRotation = rotation - tc.Rotation;
                        tc.Position = position;
                        tc.Rotation += deltaRotation;
                        tc.Scale = scale;
                    }

                }

            }

            ImGui::End();
            ImGui::PopStyleVar();
        }

        m_Panels.OnImGuiRender();



        ImGui::End();
        m_CanPick = !ImGuizmo::IsOver() && m_ViewportHovered;
    }
    void EditorLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(CR_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
        m_EditorCamera.OnEvent(event);
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.IsRepeat())
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

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
        case Key::D:
            if (control)
            {
                OnDuplicateEntity();
                return true;
            }
        case Key::P:
            if (control)
            {
                CR_INFO("Control + P");
                if (m_SceneState == SceneState::Edit)
                {
                    OnScenePlay();
                    return true;
                }
                if (m_SceneState == SceneState::Play)
                {
                    OnSceneStop();
                    return true;
                }
            }
            //Gizmo shortcuts

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
        default:
            return false;
        }

    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (!m_CanPick || Input::IsKeyPressed(Key::LeftAlt)) return false;
        if (e.getMouseButton() == 0)
        {
            m_Panels.SetSelectedEntity(m_HoveredEntity);
        }
        return true;
    }


    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_Panels.SetActiveScene(m_ActiveScene);
    }
    void EditorLayer::OpenScene()
    {
        std::filesystem::path filePath = FileDialogs::OpenFile();
        LoadScene(filePath);

    }

    void EditorLayer::LoadScene(const std::filesystem::path& path)
    {

        if (m_SceneState == SceneState::Play)
            OnSceneStop();

        if (path.extension() != ".scene")
        {
            CR_WARN("Could not load {0} - Not a scene file", path.filename().string());
            return;
        }

        if (!path.empty())
        {
            Ref<Scene> newScene = CreateRef<Scene>();
            SceneSerializer serializer(newScene);
            if (serializer.Deserialize(path))
            {
                m_EditorScene = newScene;
                m_EditorScene->OnViewportResized((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

                m_ActiveScene = m_EditorScene;
                m_Panels.SetActiveScene(m_ActiveScene);
            }

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

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;

        m_ActiveScene = Scene::Copy(m_EditorScene);

        m_ActiveScene->OnViewportResized(m_ViewportSize.x, m_ViewportSize.y);
        m_ActiveScene->OnRuntimeStart();
        m_ActiveScene->SetState(SceneState::Play);
        m_Panels.SetActiveScene(m_ActiveScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
        m_ActiveScene->OnRuntimeStop();

        m_ActiveScene = m_EditorScene;
        m_ActiveScene->SetState(SceneState::Edit);
        m_Panels.SetActiveScene(m_ActiveScene);
    }

    void EditorLayer::OnDuplicateEntity()
    {
        Entity selectedEntity = m_Panels.GetSelectedEntity();
        if (selectedEntity)
            m_EditorScene->DuplicateEntity(selectedEntity);
    }
}