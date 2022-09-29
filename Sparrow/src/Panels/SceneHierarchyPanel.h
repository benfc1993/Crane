#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"

#include "Crane/Scene/Entity.h"

#include <imgui/imgui.h>

namespace Crane {
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        template<typename T, typename F>
        void ComponentDrawer(Entity& entity, std::string title, const F& content)
        {
            if (entity.HasComponent<T>())
            {
                if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "%s", title.c_str()))
                {
                    content();
                    ImGui::TreePop();
                }
            }
        }

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}