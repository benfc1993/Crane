#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"
#include "Crane/Renderer/Shader/Texture.h"

#include <glm/gtc/type_ptr.hpp>

#include "UI/ValueDrawers/Vector.h"
#include "UI/UIComponents/Drawers.h"

namespace Crane
{
    static const std::filesystem::path s_AssetPath = "assets";

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene, bool isRequired)
        : Panel(isRequired)
    {
        m_ActiveScene = scene;
    }

    void SceneHierarchyPanel::SetActiveScene(const Ref<Scene>& scene)

    {
        m_ActiveScene = scene;
        m_SelectionContext = {};
        m_EditingEntity = {};
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {

        std::string hName = "Hierarchy###" + std::to_string(m_Index);
        ImGui::Begin(hName.c_str());

        m_ActiveScene->Reg().each([&](auto entityId)
        {
            Entity entity{ entityId, m_ActiveScene.get() };

            DrawEntityNode(entity); });

        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
        {
            m_SelectionContext = {};
            m_EditingEntity = {};
        }

        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Entity"))
            {
                Entity entity = m_ActiveScene->CreateEntity("Entity");
                m_SelectionContext = entity;
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        std::string pName = "Properties###" + std::to_string(m_Index) + "Properties";
        ImGui::Begin(pName.c_str());
        if (m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);
            ImVec4 color = ImVec4{ 0.2f, 0.8f, 0.3f, 1.0f };
            Drawers::StyledButton(color, [&]()
            {
                if (ImGui::Button("Add Component"))
                    ImGui::OpenPopup("AddComponent");

                if (ImGui::BeginPopup("AddComponent"))
                {
                    if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    {
                        if (ImGui::MenuItem("Sprite"))
                        {
                            m_SelectionContext.AddComponent<SpriteRendererComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    {
                        if (ImGui::MenuItem("Circle"))
                        {
                            m_SelectionContext.AddComponent<CircleRendererComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    if (!m_SelectionContext.HasComponent<ParticleSystem>())
                    {
                        if (ImGui::MenuItem("Particle System"))
                        {
                            m_SelectionContext.AddComponent<ParticleSystemComponent>((int)m_SelectionContext);
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!m_SelectionContext.HasComponent<CameraComponent>())
                    {
                        if (ImGui::MenuItem("Camera"))
                        {
                            m_SelectionContext.AddComponent<CameraComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!m_SelectionContext.HasComponent<RigidBody2DComponent>())
                    {
                        if (ImGui::MenuItem("Rigid Body 2D"))
                        {
                            m_SelectionContext.AddComponent<RigidBody2DComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
                    {
                        if (ImGui::MenuItem("Box Collider 2D"))
                        {
                            m_SelectionContext.AddComponent<BoxCollider2DComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                } });
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | (m_EditingEntity == entity ? ImGuiTreeNodeFlags_AllowItemOverlap : 0);

        std::string hId = "##header" + std::to_string(entity.GetUUID());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 10.0f));

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", "");

        ImGui::PopStyleVar();

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
        {
            m_EditingEntity = entity;
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



        if (m_SelectionContext == entity && m_EditingEntity == entity)
        {

            ImGui::SameLine(30.0f);
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());

            std::string id = "##" + std::to_string(entity.GetUUID());
            ImGui::SetKeyboardFocusHere();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 0.0f));
            if (ImGui::InputText(id.c_str(), buffer, sizeof(buffer), ImGuiInputTextFlags_AutoSelectAll))
            {
                auto input = std::string(buffer, sizeof(buffer));
                input.erase(input.find('\0'));
                tag = input;
            }
            ImGui::PopStyleVar();
        }
        else
        {
            ImGui::SameLine(40.0f);
            std::string name = tag.c_str();
            ImGui::Text("%s", name.c_str());

        }






        if (opened)
        {
            ImGui::TreePop();
        }
        if (entityShouldBeDelete)
        {
            m_ActiveScene->DestroyEntity(entity);
            if (entity == m_SelectionContext)
                m_SelectionContext = {};
            if (entity == m_EditingEntity)
                m_EditingEntity = {};
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
            auto& sprite = entity.GetComponent<SpriteRendererComponent>();
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(sprite.Color));
            Drawers::TextureDrawer(sprite.Texture, s_AssetPath);

            ImGui::DragFloat("Tiling Factor", &sprite.TilingFactor, 0.1f, 1.0f, 100.0f);
        });

        ComponentDrawer<CircleRendererComponent>(entity, "Circle", [&]()
        {
            auto& circle = entity.GetComponent<CircleRendererComponent>();
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(circle.Color));

            ImGui::DragFloat("Thickness", &circle.Thickness, 0.025f, 0.0f, 1.0f);
            ImGui::DragFloat("Fade", &circle.Fade, 0.00025f, 0.0f, 1.0f);
        });

        ComponentDrawer<RigidBody2DComponent>(entity, "Rigid Body 2D", [&]()
        {
            auto& component = entity.GetComponent<RigidBody2DComponent>();

            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
            const char* currentBodyType = bodyTypeStrings[(int)component.Type];

            if (ImGui::BeginCombo("Body Type", currentBodyType))
            {
                for (int i = 0; i < 3; i++)
                {
                    bool isSelected = currentBodyType == bodyTypeStrings[i];
                    if (ImGui::Selectable(bodyTypeStrings[i], isSelected, ImGuiSelectableFlags_SpanAvailWidth | ImGuiSelectableFlags_AllowItemOverlap))
                    {
                        currentBodyType = bodyTypeStrings[i];
                        component.Type = (RigidBody2DComponent::BodyType)i;
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }


                ImGui::EndCombo();
            }

            ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
        });

        ComponentDrawer<BoxCollider2DComponent>(entity, "Rigid Body 2D", [&]()
        {
            auto& component = entity.GetComponent<BoxCollider2DComponent>();

            Drawers::Vector("Offset", component.Offset);
            Drawers::Vector("Size", component.Size);
            ImGui::DragFloat("Density", &component.Density, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.1f, 0.0f, 1.0f);
        });

        ComponentDrawer<ParticleSystemComponent>(entity, "Particle System", [&]()
        {
            ParticleSystemComponent& particleComponent = entity.GetComponent<ParticleSystemComponent>();


            auto& particleData = particleComponent.Data;
            auto& particleSystem = particleComponent.System;
            int particleCount = particleSystem.GetParticleCount();
            bool isEmmitting = particleSystem.GetState();

            ImGui::ColorEdit4("Start Color", glm::value_ptr(particleData.ColorBegin));
            ImGui::ColorEdit4("End Color", glm::value_ptr(particleData.ColorEnd));

            Drawers::TextureDrawer(particleData.Texture, s_AssetPath);

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