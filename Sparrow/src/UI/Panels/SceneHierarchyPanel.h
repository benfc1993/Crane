#pragma once

#include "Panel.h"

#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"

#include "Crane/Scene/Entity.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Crane {
    class SceneHierarchyPanel : public Panel
    {
    public:
        SceneHierarchyPanel(bool isRequired = false) : Panel(isRequired) {}
        SceneHierarchyPanel(const Ref<Scene>& scene, bool isRequired = false);

        virtual void SetActiveScene(const Ref<Scene>& scene) override;

        virtual void OnImGuiRender() override;

        virtual void SetSelectedEntity(Entity entity) override { m_SelectionContext = entity; }
        virtual Entity GetSelectedEntity() const override { return m_SelectionContext; }

    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);

    private:
        template<typename T, typename ContentFn>
        void ComponentDrawer(Entity& entity, std::string title, const ContentFn& contentFn, bool canDelete = true)
        {
            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
            if (entity.HasComponent<T>())
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

                bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", title.c_str());

                bool removeComponent = false;
                if (canDelete)
                {
                    ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
                    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                    ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);

                    if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
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
                ImGui::PopStyleVar(2);

                if (opened)
                {
                    contentFn();
                    ImGui::Separator();
                    ImGui::TreePop();
                }

                if (removeComponent == true)
                    entity.RemoveComponent<T>();
            }
        }

    private:
        Entity m_SelectionContext;
        Entity m_EditingEntity;
    };
}