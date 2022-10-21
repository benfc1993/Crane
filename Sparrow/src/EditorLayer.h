#pragma once

#include <Crane.h>

#include "Crane/Scene/Serialization/SceneSerializer.h"
#include "Crane/Camera/EditorCamera.h"

#include "EditorSettings.h"

#include "UI/Panels/Panels.h"
#include "UI/Panels/RenderStatsPanel.h"
#include "UI/Panels/SceneHierarchyPanel.h"
#include "UI/Panels/SettingsPanel/SettingsPanel.h"
#include "UI/Panels/ContentBrowserPanel.h"
#include "UI/ToolBars/SceneToolbar.h"

namespace Crane {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;
        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(Time time) override;
        virtual void OnImGuiRender() override;
        void OnOverlayRender();
        virtual void OnEvent(Event& event) override;

        void OnScenePlay();
        void OnSceneStop();

        void LoadScene(const std::filesystem::path& path);
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();

        void OnDuplicateEntity();
    private:
        Ref<VertexArray> m_SquareVertexArray;
        Ref<VertexArray> m_TriangleVertexArray;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene = nullptr;
        Ref<Scene> m_EditorScene = nullptr, m_RuntimeScene = nullptr;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;
        Entity m_HoveredEntity;

        bool m_PrimaryCamera = true;

        bool m_CanPick = true;

        EditorCamera m_EditorCamera;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        bool m_ViewportFocused = false, m_ViewportHovered = false;

        SceneState m_SceneState = SceneState::Edit;

        Ref<EditorSettings> m_EditorSettings = CreateRef<EditorSettings>();

        //Panels
        Panels m_Panels;
        Ref<SettingsPanel> m_SettingsPanel = nullptr;
        // RenderStatsPanel m_RenderStatsPanel;

        //Global State
        bool m_ShowSettings = false;

        //Gizmos
        int m_GizmoType = -1;
    };
}