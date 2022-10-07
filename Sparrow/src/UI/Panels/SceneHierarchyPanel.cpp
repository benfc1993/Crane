#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

#include "UI/ValueDrawers/Vector.h"

namespace Crane
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
        m_SelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");

        m_Context->Reg().each([&](auto entityId)
        {
            Entity entity{ entityId, m_Context.get() };

            DrawEntityNode(entity); });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Entity"))
                m_Context->CreateEntity("Entity");

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
            ImVec4 color = ImVec4{ 0.2f, 0.8f, 0.3f, 1.0f };
            StyledButton(color, [&]()
            {
                if (ImGui::Button("Add Component"))
                    ImGui::OpenPopup("AddComponent");

                if (ImGui::BeginPopup("AddComponent"))
                {

                    if (ImGui::MenuItem("Sprite"))
                    {
                        m_SelectionContext.AddComponent<SpriteRendererComponent>();
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::MenuItem("Particle System"))
                    {
                        m_SelectionContext.AddComponent<ParticleSystemComponent>();
                        ImGui::CloseCurrentPopup();
                    }

                    if (ImGui::MenuItem("Camera"))
                    {
                        m_SelectionContext.AddComponent<CameraComponent>();
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                } });
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        bool entityShouldBeDelete = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                entityShouldBeDelete = true;
            }

            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGui::TreePop();
        }
        if (entityShouldBeDelete)
        {
            m_Context->DestroyEntity(entity);
            if (entity == m_SelectionContext)
                m_SelectionContext = {};
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {

        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            ImGui::Text("Tag");
            if (ImGui::InputText("##", buffer, sizeof(buffer)))
            {
                auto input = std::string(buffer, sizeof(buffer));
                input.erase(input.find('\0'));
                tag = input;
            }
            ImGui::Separator();
        }

        ComponentDrawer<TransformComponent>(
            entity, "Transform", [&]()
        {
            auto& transform = entity.GetComponent<TransformComponent>();

            glm::vec2 test = { 1.0f, 0.0f };
            Drawers::Vector("Position", transform.Position);
            glm::vec3 rotation = glm::degrees(transform.Rotation);
            Drawers::Vector("Rotation", rotation);
            transform.Rotation = glm::radians(rotation);
            Drawers::Vector("Scale", transform.Scale);
        },
            false);

        ComponentDrawer<CameraComponent>(entity, "Camera", [&]()
        {
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;

            ImGui::Checkbox("Primary Camera", &cameraComponent.Primary);

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];


            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected, ImGuiSelectableFlags_SpanAvailWidth | ImGuiSelectableFlags_AllowItemOverlap))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectionType((SceneCamera::ProjectionType)i);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }


                ImGui::EndCombo();
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float fov = camera.GetPerspectiveVerticalFov();
                float nearClip = camera.GetPerspectiveNearClip();
                float farClip = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Vectical FOV", &fov))
                    camera.SetPerspectiveVerticalFov(fov);

                if (ImGui::DragFloat("Near clip", &nearClip))
                    camera.SetPerspectiveNearClip(nearClip);

                if (ImGui::DragFloat("Far clip", &farClip))
                    camera.SetPerspectiveFarClip(farClip);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float size = camera.GetOrthographicSize();
                float nearClip = camera.GetOrthographicNearClip();
                float farClip = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Size", &size))
                    camera.SetOrthographicSize(size);

                if (ImGui::DragFloat("Near clip", &nearClip))
                    camera.SetOrthographicNearClip(nearClip);

                if (ImGui::DragFloat("Far clip", &farClip))
                    camera.SetOrthographicFarClip(farClip);

            } });

        ComponentDrawer<SpriteRendererComponent>(entity, "Sprite", [&]()
        {
            auto& spriteColor = entity.GetComponent<SpriteRendererComponent>().Color;
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(spriteColor)); });

        ComponentDrawer<ParticleSystemComponent>(entity, "Particle System", [&]()
        {
            ParticleSystemComponent& particleComponent = entity.GetComponent<ParticleSystemComponent>();


            auto& particleData = particleComponent.Data;
            auto& particleSystem = particleComponent.System;
            int particleCount = particleSystem.GetParticleCount();
            bool isEmmitting = particleSystem.GetState();

            ImGui::ColorEdit4("Start Color", glm::value_ptr(particleData.ColorBegin));
            ImGui::ColorEdit4("End Color", glm::value_ptr(particleData.ColorEnd));

            ImGui::DragFloat("Life Time", &particleData.Lifetime, 0.1f, 0.0f, 1000.0f);
            Drawers::Range("Life Time Variation", particleData.LifetimeVariation, 0.0f, 1.0f);

            ImGui::InputFloat("Initial Size", &particleData.SizeBegin);
            ImGui::InputFloat("End Size", &particleData.SizeEnd);
            Drawers::Range("Size Variation", particleData.SizeVariation, 0.0f, 1.0f);

            Drawers::Vector("Initial Velocity", particleData.Velocity);
            Drawers::Range("Velocity Variation", particleData.VelocityVariation, 0.0f, 1.0f);

            ImGui::InputInt("Burst Size", &particleData.BurstSize);
            if (ImGui::InputInt("Max Particles", &particleCount))
            {
                if (particleCount > 0)
                {
                    particleSystem.SetParticleCount(particleCount);
                }
            }
            if (ImGui::Checkbox("Emit", &isEmmitting))
                particleSystem.SetActive(isEmmitting); });
    }
}