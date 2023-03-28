#include "ImGuizmoLayer.h"

#include "Crane/Input/Input.h"
#include "Crane/Math/Math.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>


namespace Crane {
    void ImGuizmoLayer::HandleInput()
    {
        if (Input::IsKeyPressed(Key::Q))
            m_GizmoType = -1;
        if (Input::IsKeyPressed(Key::W))
            m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
        if (Input::IsKeyPressed(Key::E))
            m_GizmoType = ImGuizmo::OPERATION::SCALE;
        if (Input::IsKeyPressed(Key::R))
            m_GizmoType = ImGuizmo::OPERATION::ROTATE;
    }

    bool ImGuizmoLayer::OnUpdate(Entity selectedEntity)
    {
        HandleInput();
        bool usingGuizmo = m_GizmoType != -1 && (ImGuizmo::IsUsing() || ImGuizmo::IsOver());
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = ImGui::GetWindowWidth();
            float windowHeight = ImGui::GetWindowHeight();
            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Edit camera
            const glm::mat4& cameraProjection = m_camera.GetProjection();
            const glm::mat4& cameraView = m_camera.GetViewMatrix();

            //Entity transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.Transform();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float translationSnapValue = 0.1f;
            float rotationSnapValue = 45.0f;
            float snapValue = m_GizmoType == ImGuizmo::OPERATION::ROTATE ? rotationSnapValue : translationSnapValue;

            float snapValues[3] = { snapValue, snapValue, snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);



            if (usingGuizmo)
            {

                glm::vec3 parentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 parentScale = glm::vec3(1.0f, 1.0f, 1.0f);

                auto& hc = selectedEntity.GetComponent<HierarchyComponent>();

                if (hc.Parent != 0)
                {
                    Entity parent = m_scene->GetEntityByUUID(hc.Parent);
                    auto& parentTransform = parent.GetComponent<TransformComponent>();
                    parentPosition = parentTransform.WorldPosition;
                    parentScale = parentTransform.WorldScale;
                }

                glm::vec3 position, rotation, scale;
                Math::DecomposeTransform(transform, position, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Position = position - parentPosition;
                tc.Rotation += deltaRotation;
                tc.Scale = scale / parentScale;
            }
        }
        return usingGuizmo;
    }

}