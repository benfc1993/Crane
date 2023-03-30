#pragma once 

#include "Crane/Camera/EditorCamera.h"
#include "Crane/Scene/Entity.h"

#include <imgui/imgui_internal.h>

namespace Crane {
	class ImGuizmoLayer
	{
	public:
		ImGuizmoLayer(EditorCamera* camera, Ref<Scene> scene): m_camera(camera), m_scene(scene) {}
		bool OnUpdate(Entity selectedEntity, glm::vec2 viewportBounds[2]);

	private:
		void HandleInput();

	private:
		EditorCamera* m_camera;
		Ref<Scene> m_scene;
		int m_GizmoType = -1;
		int m_GizmoMode = 0;
	};
}