#pragma once

#include "ImGuizmoLayer.h"
#include "Viewport.h"
#include "EditorLayer.h"
#include "Crane/Camera/EditorCamera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Crane {
    class EditorViewport: public Viewport
    {
    public:
        EditorViewport(std::string name, Ref<Scene> scene, EditorLayer* editorLayer);
        virtual void OnImGuiRender() override;
        virtual void OnUpdate(Time time) override;
        virtual void OnEvent(Event& event) override;
        void OnOverlayRender();
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

        virtual void SetSelectedEntity(Entity entity) override
        {
            m_SelectedEntity = entity;
        }
        virtual Entity GetSelectedEntity() const override { return m_SelectedEntity; }

    private:
        EditorCamera m_camera;

        EditorLayer* m_editorLayer;
        Ref<ImGuizmoLayer> m_imGuizmoLayer;

        Entity m_HoveredEntity;

        bool m_CanPick = false;
    };
}