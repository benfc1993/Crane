#pragma once

#include "UI/Panels/Panel.h"
#include "Crane/Scene/Scene.h"
#include "Crane/Renderer/Framebuffer.h"


namespace Crane {
    class Viewport: public Panel
    {
    public:
        Viewport(std::string name, Ref<Scene> scene): m_Name(name), m_Scene(scene), Panel() {}
        virtual void OnImGuiRender() {}
        virtual void OnUpdate(Time time) {}
        virtual void OnEvent(Event& event) {}
        void SetIndex(int index) { m_Index = index; }


    protected:
        virtual void SetSelectedEntity(Entity entity) override
        {
            m_SelectedEntity = entity;
        }
        virtual Entity GetSelectedEntity() const override { return m_SelectedEntity; }

    protected:
        std::string m_Name;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];

        Ref<Framebuffer> m_Framebuffer;
        Ref<Scene> m_Scene;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;

        Entity m_SelectedEntity;
        int m_Index = -1;
    };
}