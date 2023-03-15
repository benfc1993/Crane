#include "SceneHierarchyPanel.h"

#include "Crane/Scene/Components.h"
#include "Crane/Renderer/Shader/Texture.h"
#include "Crane/Scripting/ScriptEngine.h"
#include "Crane/Scene/Hierarchy.h"

#include <glm/gtc/type_ptr.hpp>

#include "UI/ValueDrawers/Vector.h"
#include "UI/UIComponents/Drawers.h"

namespace Crane
{
    static const std::filesystem::path s_AssetPath = "assets";
    static EntityAction entityAction = EntityAction::None;
    static UUID toActOn = UUID(0);
    static UUID toMove = UUID(0);

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


        if (entityAction != EntityAction::Moving && entityAction != EntityAction::Move)
        {
            toActOn = UUID(0);
            toMove = UUID(0);
            entityAction = EntityAction::None;
        }

        ImGui::BeginGroup();
        m_ActiveScene->Reg().each([&](auto entityId)
        {
            Entity entity{ entityId, m_ActiveScene.get() };
            if (m_ActiveScene->EntityExists(entity.GetUUID()))
                if (entity.GetComponent<HierarchyComponent>().Parent == 0)
                {
                    DrawEntityNode(entity, 0);
                }
        });

        ImGui::EndGroup();
        ImVec2 region = ImGui::GetContentRegionAvail();
        ImGui::InvisibleButton("bg", region, ImGuiButtonFlags_AllowItemOverlap);

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
            {
                toActOn = UUID(0);
                entityAction = EntityAction::Move;
                ImGui::EndDragDropTarget();
            }
        }
        if (entityAction == EntityAction::Delete)
        {
            Entity entity = m_ActiveScene->GetEntityByUUID(toActOn);
            Hierarchy::Delete(m_ActiveScene, entity);
            m_ActiveScene->DestroyEntity(entity);
            if (entity == m_SelectionContext)
                m_SelectionContext = {};
            if (entity == m_EditingEntity)
                m_EditingEntity = {};
        }

        if (entityAction == EntityAction::AddChild)
        {
            Entity entity = m_ActiveScene->GetEntityByUUID(toActOn);
            Entity newChild = m_ActiveScene->CreateEntity("Entity");
            Hierarchy::AddChild(m_ActiveScene, entity, newChild);
        }

        if (entityAction == EntityAction::AddParent)
        {
            Entity entity = m_ActiveScene->GetEntityByUUID(toActOn);
            Entity newParent = m_ActiveScene->CreateEntity("Entity");
            Hierarchy::AddParent(m_ActiveScene, entity, newParent);
        }

        if (entityAction == EntityAction::Move)
        {
            Entity parent = m_ActiveScene->GetEntityByUUID(toActOn);
            Entity child = m_ActiveScene->GetEntityByUUID(toMove);
            Hierarchy::Move(m_ActiveScene, child, parent);

            auto& childTransform = child.GetComponent<TransformComponent>();
            if (parent.IsNull())
            {
                childTransform.Position = childTransform.WorldPosition;
                childTransform.Scale = childTransform.WorldScale;
            }
            else
            {
                auto& parentTransform = parent.GetComponent<TransformComponent>();

                childTransform.Position = childTransform.WorldPosition - parentTransform.WorldPosition;
                childTransform.Scale = childTransform.WorldScale / parentTransform.WorldScale;
            }

            entityAction = EntityAction::None;
        }

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

                    if (!m_SelectionContext.HasComponent<CircleColliderComponent>())
                    {
                        if (ImGui::MenuItem("Circle Collider"))
                        {
                            m_SelectionContext.AddComponent<CircleColliderComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    if (!m_SelectionContext.HasComponent<ScriptComponent>())
                    {
                        if (ImGui::MenuItem("Script Component"))
                        {
                            m_SelectionContext.AddComponent<ScriptComponent>();
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndPopup();
                } });
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity, float level)
    {


        auto& tag = entity.GetComponent<TagComponent>().Tag;
        bool hasChild = entity.GetComponent<HierarchyComponent>().First != 0;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | (m_EditingEntity == entity ? ImGuiTreeNodeFlags_AllowItemOverlap : 0) | (hasChild ? 0 : ImGuiTreeNodeFlags_Leaf);

        std::string hId = "##header" + std::to_string(entity.GetUUID());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 10.0f));

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", "");

        if (ImGui::BeginDragDropSource())
        {
            toMove = entity.GetUUID();
            entityAction = EntityAction::Moving;

            ImGui::SetDragDropPayload("ENTITY", NULL, 0);
            ImGui::Text("%s", entity.GetName().c_str());
            ImGui::EndDragDropSource();
        }
        else if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
            {
                toActOn = entity.GetUUID();
                entityAction = EntityAction::Move;
                ImGui::EndDragDropTarget();
            }
        }

        ImGui::PopStyleVar();

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
        {
            m_EditingEntity = entity;
        }

        if (ImGui::BeginPopupContextItem())
        {

            if (ImGui::MenuItem("Add Child Entity"))
            {
                toActOn = entity.GetUUID();
                entityAction = EntityAction::AddChild;
            }

            if (ImGui::MenuItem("Add Parent"))
            {
                toActOn = entity.GetUUID();
                entityAction = EntityAction::AddParent;
            }

            if (ImGui::MenuItem("Delete Entity"))
            {
                toActOn = entity.GetUUID();
                entityAction = EntityAction::Delete;
            }

            ImGui::EndPopup();
        }

        if (m_SelectionContext == entity && m_EditingEntity == entity)
        {

            ImGui::SameLine(30.0f + (10 * level));
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
            ImGui::SameLine(40.0f + (10 * level));
            std::string name = tag.c_str();
            ImGui::Text("%s", name.c_str());

        }

        if (opened)
        {
            auto& hc = entity.GetComponent<HierarchyComponent>();

            UUID curr = hc.First;
            while (curr != 0)
            {
                DrawEntityNode(m_ActiveScene->GetEntityByUUID(curr), (level + 1.0f));
                if (!m_ActiveScene->EntityExists(curr)) break;
                auto next = m_ActiveScene->GetEntityByUUID(curr).GetComponent<HierarchyComponent>().Next;
                curr = next;
            }

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
                auto input = std::string(buffer, sizeof(buffer));
                input.erase(input.find('\0'));
                tag = input;
            }
            ImGui::Separator();
        }

        ComponentDrawer<TransformComponent>(
            entity, m_ActiveScene, "Transform", [&]()
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& hc = entity.GetComponent<HierarchyComponent>();

            glm::vec2 test = { 1.0f, 0.0f };

            if (hc.Parent == 0)
            {
                Drawers::Vector("Position", transform.Position);
                Drawers::Vector("Scale", transform.Scale);
            }
            else
            {
                static bool local = false;
                ImGui::Checkbox("Local", &local);
                if (local)
                {
                    Drawers::Vector("Local Position", transform.Position);
                    Drawers::Vector("Local Scale", transform.Scale);
                }
                else
                {
                    Entity parent = m_ActiveScene->GetEntityByUUID(hc.Parent);
                    auto& parentTransform = parent.GetComponent<TransformComponent>();

                    glm::vec3 parentPosition = parentTransform.WorldPosition;
                    glm::vec3 worldPosition = transform.WorldPosition;
                    if (Drawers::Vector("Position", worldPosition))
                    {
                        transform.Position = worldPosition - parentPosition;
                    }

                    glm::vec3 parentScale = parentTransform.WorldScale;
                    glm::vec3 worldScale = transform.WorldScale;
                    if (Drawers::Vector("Scale", worldScale))
                        transform.Scale = worldScale / parentScale;

                }
            }

            glm::vec3 rotation = glm::degrees(transform.Rotation);
            Drawers::Vector("Rotation", rotation);
            transform.Rotation = glm::radians(rotation);
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

        ComponentDrawer<BoxCollider2DComponent>(entity, "Box Collider", [&]()
        {
            auto& component = entity.GetComponent<BoxCollider2DComponent>();

            Drawers::Vector("Size", component.Size);
            Drawers::Vector("Offset", component.Offset);
            ImGui::DragFloat("Density", &component.Density, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &component.Friction, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution", &component.Restitution, 0.1f, 0.0f, 1.0f);
            ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.1f, 0.0f, 1.0f);
        });

        ComponentDrawer<CircleColliderComponent>(entity, "Circle Collider", [&]()
        {
            auto& component = entity.GetComponent<CircleColliderComponent>();

            Drawers::Vector("Offset", component.Offset);
            ImGui::DragFloat("Radius", &component.Radius);
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

        ComponentDrawerTitle<ScriptComponent>(entity, [&]() {return entity.GetComponent<ScriptComponent>().ScriptName;}, [&]()
        {
            auto& component = entity.GetComponent<ScriptComponent>();

            std::string currentScript = component.FullName;
            if (ImGui::BeginCombo("Script", component.ScriptName.c_str()))
            {
                auto scriptMap = ScriptEngine::GetScripts();
                for (auto script : scriptMap)
                {
                    std::string fullName = script.second->GetFullName();
                    std::string scriptName = script.second->GetName();
                    bool isSelected = currentScript == fullName;
                    if (ImGui::Selectable(scriptName.c_str(), isSelected, ImGuiSelectableFlags_SpanAvailWidth | ImGuiSelectableFlags_AllowItemOverlap))
                    {
                        currentScript = fullName.c_str();
                        component.SetScript(fullName);
                    }
                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }


                ImGui::EndCombo();
            }

            if (component.FullName == "") return;

            Ref<ScriptClass> script = ScriptEngine::GetScript(component.FullName);
            script->GetFields();


        });
    }

}