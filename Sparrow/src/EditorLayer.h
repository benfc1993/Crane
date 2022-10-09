#pragma once

#include <Crane.h>

#include "Crane/Scene/Serialization/SceneSerializer.h"
#include "Crane/Camera/EditorCamera.h"

#include "UI/Panels/Panels.h"
#include "UI/Panels/RenderStatsPanel.h"
#include "UI/Panels/SceneHierarchyPanel.h"
#include "UI/Panels/SettingsPanel/SettingsPanel.h"

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
        virtual void OnEvent(Event& event) override;
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        void NewScene();
        void OpenScene();
        void SaveScene();
        void SaveSceneAs();
    private:
        Ref<VertexArray> m_SquareVertexArray;
        Ref<VertexArray> m_TriangleVertexArray;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene = nullptr;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;
        Entity m_HoveredEntity;

        bool m_PrimaryCamera = true;

        bool m_CanPick = true;

        EditorCamera m_EditorCamera;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        bool m_ViewportFocused = false, m_ViewportHovered = false;


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