#pragma once 

#include "Crane/Camera/EditorCamera.h"
#include "Crane/Scene/Entity.h"

namespace Crane {
	class ImGuizmoLayer
	{
	public:
		ImGuizmoLayer(EditorCamera camera, Ref<Scene> scene): m_camera(camera), m_scene(scene) {}
		std::function<void(Entity selectedEntity, glm::vec2 viewportBoundsMin, glm::vec2 viewportBoundsMax)> OnUpdate();

	private:
		void HandleInput();

	private:
		EditorCamera m_camera;
		Ref<Scene> m_scene;
		int m_GizmoType = -1;
	};
}