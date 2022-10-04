#include "crpch.h"

#include "OrthographicCameraController.h"

#include "Crane/Input/Input.h"
#include "Crane/Input/KeyCodes.h"


namespace Crane {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
    }

    void OrthographicCameraController::OnUpdate(Time time)
    {
        CR_PROFILE_FUNCTION();

        m_CameraTranslationSpeed = m_ZoomLevel;

        m_CameraTranslationSpeed = std::max(m_CameraTranslationSpeed, 0.5f);

        if (Input::IsKeyPressed(Key::A))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(Key::D))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(Key::W))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(Key::S))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * time.DeltaTime();
        }


        m_Camera.SetPosition(m_CameraPosition);

        if (!m_Rotation) return;

        if (Input::IsKeyPressed(Key::R))
        {
            m_CameraRotation = 0;
            m_Camera.SetRotation(m_CameraRotation);
            return;
        }

        if (Input::IsKeyPressed(Key::Q))
        {
            m_CameraRotation -= m_CameraRotationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(Key::E))
        {
            m_CameraRotation += m_CameraRotationSpeed * time.DeltaTime();
        }

        m_Camera
            .SetRotation(m_CameraRotation);
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        CR_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(CR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }


    bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        m_ZoomLevel -= event.GetYOffset() * 0.12f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.2f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}
