#pragma once 

#include "Crane/Camera/EditorCamera.h"
#include "Crane/Scene/Entity.h"

#include <imgui/imgui.h>
#include <ImGuizmo.h>

namespace Crane {
	class ImGuizmoLayer
	{
	public:
		ImGuizmoLayer(EditorCamera camera, Ref<Scene> scene): m_camera(camera), m_scene(scene) {}
		bool OnUpdate(Entity selectedEntity, glm::vec2 viewportBoundsMin, glm::vec2 viewportBoundsMax);

	private:
		void HandleInput();

	private:
		EditorCamera m_camera;
		Ref<Scene> m_scene;
		int m_GizmoType = -1;
	};
}