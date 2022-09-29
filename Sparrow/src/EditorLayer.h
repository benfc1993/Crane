#pragma once

#include <Crane.h>
#include "Panels/RenderStatsPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ParticleSystemPropertiesPanel.h"

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
        OrthographicCameraController m_CameraController;

        Ref<Shader> m_Shader;
        Ref<Texture2D> m_Texture;


        glm::vec4 m_Color{ 0.8f, 0.2f, 0.3f, 1.0f };
        glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
        glm::vec2 m_Scale{ 1.0f, 1.0f };
        float m_Angle = 0.0f;
        Ref<VertexArray> m_SquareVertexArray;
        Ref<VertexArray> m_TriangleVertexArray;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene = nullptr;
        Entity m_QuadEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        bool m_PrimaryCamera = true;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        bool m_ViewportFocused = false, m_ViewportHovered = false;

        //Panels
        ParticleSystemPropertiesPanel m_ParticlePropertiesPanel;
        RenderStatsPanel m_RenderStatsPanel;
        SceneHierarchyPanel m_HierarchyPanel;
    };
}