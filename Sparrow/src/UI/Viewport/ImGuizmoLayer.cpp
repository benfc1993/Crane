#include "ImGuizmoLayer.h"

#include "Crane/Input/Input.h"
#include "Crane/Math/Math.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include <ImGuizmo.h>

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

	bool ImGuizmoLayer::OnUpdate(Entity selectedEntity, glm::vec2 viewportBounds[2])
	{
		HandleInput();

		ImGui::PushID(1);
		ImGui::BeginMenuBar();
		if (m_GizmoType != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", m_GizmoMode == ImGuizmo::LOCAL))
				m_GizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", m_GizmoMode == ImGuizmo::WORLD))
				m_GizmoMode = ImGuizmo::WORLD;
		}
		ImGui::EndMenuBar();
		ImGui::PopID();

		bool usingGuizmo = m_GizmoType != -1 && (ImGuizmo::IsUsing() || ImGuizmo::IsOver());
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);
			// Edit camera
			const glm::mat4& cameraProjection = m_camera->GetProjection();
			const glm::mat4& cameraView = m_camera->GetViewMatrix();

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
				(ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)m_GizmoMode, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);


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
	};
}