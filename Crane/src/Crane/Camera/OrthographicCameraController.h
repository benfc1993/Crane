#pragma once

#include "OrthographicCamera.h"

#include "Crane/Core/Time.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Events/MouseEvent.h"


namespace Crane {
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = true);

        void OnUpdate(Time time);
        void OnEvent(Event& e);

        void OnResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; };
        const OrthographicCamera& GetCamera() const { return m_Camera; };
    private:
        bool OnMouseScrolledEvent(MouseScrolledEvent& event);
        bool OnResizeEvent(WindowResizeEvent& event);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_Rotation;
        glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_CameraRotation = 0.0f;
        float m_CameraTranslationSpeed = 10.0f;
        float m_CameraRotationSpeed = 30.0f;
    };
}