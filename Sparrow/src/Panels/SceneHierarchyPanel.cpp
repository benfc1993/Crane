#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>

#include "ValueDrawers/ValueDrawers.h"


namespace Crane {
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
    {
        m_Context = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");

        m_Context->Reg().each([&](auto entityId)
            {
                Entity entity{ entityId, m_Context.get() };

                DrawEntityNode(entity);
            });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (opened)
        {
            ImGui::TreePop();
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
                tag = std::string(buffer, sizeof(buffer));
            }
            ImGui::Separator();
        }

        ComponentDrawer<TransformComponent>(entity, "Transform", [&]() {
            auto& transform = entity.GetComponent<TransformComponent>();

            Drawers::Vector3("Position", transform.Position);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation));
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale));
            });

        ComponentDrawer<CameraComponent>(entity, "Camera", [&]() {
            auto& camera = entity.GetComponent<CameraComponent>().Camera;

            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
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
                float near = camera.GetPerspectiveNearClip();
                float far = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("Vectical FOV", &fov))
                    camera.SetPerspectiveVerticalFov(fov);

                if (ImGui::DragFloat("Near clip", &near))
                    camera.SetPerspectiveNearClip(near);

                if (ImGui::DragFloat("Far clip", &far))
                    camera.SetPerspectiveFarClip(far);
            }

            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float size = camera.GetOrthographicSize();
                float near = camera.GetOrthographicNearClip();
                float far = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Size", &size))
                    camera.SetOrthographicSize(size);

                if (ImGui::DragFloat("Near clip", &near))
                    camera.SetOrthographicNearClip(near);

                if (ImGui::DragFloat("Far clip", &far))
                    camera.SetOrthographicFarClip(far);

            }
            });

        ComponentDrawer<SpriteRendererComponent>(entity, "Sprite", [&]() {
            auto& spriteColor = entity.GetComponent<SpriteRendererComponent>().Color;
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(spriteColor));
            });

        ComponentDrawer<ParticleSystemComponent>(entity, "Particle System", [&]() {
            ParticleSystemComponent& particleComponent = entity.GetComponent<ParticleSystemComponent>();


            auto& particleData = particleComponent.Data;
            auto& particleSystem = particleComponent.System;
            int particleCount = particleSystem.GetParticleCount();
            bool isEmmitting = particleSystem.GetState();

            ImGui::ColorEdit4("Start Color", glm::value_ptr(particleData.ColorBegin));
            ImGui::ColorEdit4("End Color", glm::value_ptr(particleData.ColorEnd));

            ImGui::DragFloat("Life Time", &particleData.Lifetime, 0.1f, 0.0f, 1000.0f);
            ImGui::DragFloat("Life Time Variation", &particleData.LifetimeVariation, 0.1f, 0.0f, 1.0f);

            ImGui::InputFloat("Initial Size", &particleData.SizeBegin);
            ImGui::InputFloat("End Size", &particleData.SizeEnd);
            ImGui::DragFloat("Size Variation", &particleData.SizeVariation, 0.1f, 0.0f, 1.0f);

            ImGui::InputFloat3("Initial Velocity", glm::value_ptr(particleData.Velocity));
            ImGui::DragFloat3("Velocity Variation", glm::value_ptr(particleData.VelocityVariation), 0.1f, 0.0f, 1.0f);

            ImGui::InputInt("Burst Size", &particleData.BurstSize);
            if (ImGui::InputInt("Max Particles", &particleCount))
            {
                if (particleCount > 0)
                {
                    particleSystem.SetParticleCount(particleCount);
                }
            }
            if (ImGui::Checkbox("Emit", &isEmmitting))
                particleSystem.SetActive(isEmmitting);
            });
    }
}