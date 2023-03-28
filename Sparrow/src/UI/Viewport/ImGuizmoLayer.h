#pragma once 

#include "Crane/Camera/EditorCamera.h"
#include "Crane/Scene/Entity.h"

namespace Crane {
    class ImGuizmoLayer
    {
    public:
        ImGuizmoLayer(EditorCamera camera, Ref<Scene> scene): m_camera(camera), m_scene(scene) {}
        bool OnUpdate(Entity selectedEntity);

    private:
        void HandleInput();

    private:
        EditorCamera m_camera;
        Ref<Scene> m_scene;
        int m_GizmoType = -1;
        glm::vec2 m_ViewportBounds[2];
    };
}