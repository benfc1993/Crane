#pragma once

#include "Panel.h"

#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"

#include "Crane/Scene/Entity.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Crane {

    enum class EntityAction
    {
        None = 0,
        AddParent = 1,
        AddChild = 2,
        Delete = 3,
        Move = 4,
        Moving = 5
    };

    class SceneHierarchyPanel: public Panel
    {
    public:
        SceneHierarchyPanel(bool isRequired = false): Panel(isRequired) {}
        SceneHierarchyPanel(const Ref<Scene>& scene, bool isRequired = false);

        virtual void SetActiveScene(const Ref<Scene>& scene) override;

        virtual void OnImGuiRender() override;

        virtual void SetSelectedEntity(Entity entity) override
        {
            m_SelectionContext = entity;
        }
        virtual Entity GetSelectedEntity() const override { return m_SelectionContext; }

    private:
        void DrawEntityNode(Entity entity, float level);
        void DrawComponents(Entity entity);

    private:
        template<typename T, typename ContentFn>
        void ComponentDrawer(Entity& entity, std::string title, const ContentFn& contentFn, bool canDelete = true)
        {
            ComponentDrawer<T>(entity, nullptr, title, contentFn, canDelete);
        }
        template<typename T, typename ContentFn>
        void ComponentDrawer(Entity& entity, Ref<Scene> scene, std::string title, const ContentFn& contentFn, bool canDelete = true)
        {
            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap;
            if (entity.HasComponent<T>())
            {
                ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
                float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                ImGui::Separator();
                bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", title.c_str());
                ImGui::PopStyleVar();
                ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

                std::string popupName = "ComponentSettings" + std::to_string(typeid(T).hash_code());

                if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
                {
                    ImGui::OpenPopup(popupName.c_str());
                }

                bool removeComponent = false;
                if (ImGui::BeginPopup(popupName.c_str()))
                {
                    if (ImGui::MenuItem("Remove component"))
                        removeComponent = true;

                    ImGui::EndPopup();
                }

                if (open)
                {
                    contentFn();
                    ImGui::TreePop();
                }

                if (removeComponent == true)
                    entity.RemoveComponent<T>();
            }
        }
        template<typename T, typename TitleFn, typename ContentFn>
        void ComponentDrawerTitle(Entity& entity, const TitleFn& titleFn, const ContentFn& contentFn, bool canDelete = true)
        {
            if (entity.HasComponent<T>())
            {
                std::string title = titleFn();
                ComponentDrawer<T>(entity, title, contentFn, canDelete);
            }
        }

    private:
        Entity m_SelectionContext;
        Entity m_EditingEntity;
    };
}