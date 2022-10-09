#pragma once

#include "Crane/Core/Base.h"

#include "Crane/Scene/Scene.h"
#include "Panel.h"

namespace Crane {
    class Panels
    {
    public:
        Panels() {}

        void OnImGuiRender()
        {
            for (auto panel : m_Panels)
            {
                panel->OnImGuiRender();
            }
        }



        template <typename T, typename ...Args>
        void AddPanel(Args&&... args)
        {
            auto panel = CreateRef<T>(std::forward<Args>(args)...);
            AddPanel(panel);
        }

        void AddPanel(const Ref<Panel>& panel)
        {
            m_Panels.push_back(panel);
            panel->SetIndex(m_Panels.size() - 1);
            panel->SetActiveScene(m_ActiveScene);
        }

        void RemovePanel(const int index)
        {
            auto panel = m_Panels[index];
            if (!panel->IsRequired())
                m_Panels.erase(m_Panels.begin() + index);
        }

        void SetActiveScene(const Ref<Scene>& scene)
        {
            m_ActiveScene = scene;
            for (auto panel : m_Panels)
            {
                panel->SetActiveScene(m_ActiveScene);
            }
        };

        void SetSelectedEntity(Entity entity)
        {
            for (auto panel : m_Panels)
            {
                panel->SetSelectedEntity(entity);
            }
        };

        Entity GetSelectedEntity()
        {
            Entity blank = Entity{};
            for (auto panel : m_Panels)
            {
                auto entity = panel->GetSelectedEntity();
                if (entity != blank)
                    return entity;
            }
            return blank;
        };
    private:
        std::vector<Ref<Panel>> m_Panels;
        Ref<Scene> m_ActiveScene = nullptr;
    };
}