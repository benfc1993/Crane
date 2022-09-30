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
        void ComponentDrawer(Entity& entity, std::string title, const F& content, bool canDelete = true)
        {
            if (entity.HasComponent<T>())
            {
                bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap, "%s", title.c_str());
                ImGui::SameLine();

                bool removeComponent = false;
                if (canDelete)
                {
                    if (ImGui::Button("..."))
                    {
                        ImGui::OpenPopup("ComponentSettings");
                    }

                    if (ImGui::BeginPopup("ComponentSettings"))
                    {
                        if (ImGui::MenuItem("Remove Component"))
                            removeComponent = true;
                        ImGui::EndPopup();
                    }
                }

                if (opened)
                {
                    content();
                    ImGui::TreePop();
                }

                if (removeComponent == true)
                    entity.RemoveComponent<T>();
            }
        }

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}