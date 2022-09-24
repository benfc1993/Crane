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

        if (Input::IsKeyPressed(CR_KEY_A))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(CR_KEY_D))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(CR_KEY_W))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(CR_KEY_S))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * time.DeltaTime();
        }


        m_Camera.SetPosition(m_CameraPosition);

        if (!m_Rotation) return;

        if (Input::IsKeyPressed(CR_KEY_R))
        {
            m_CameraRotation = 0;
            m_Camera.SetRotation(m_CameraRotation);
            return;
        }

        if (Input::IsKeyPressed(CR_KEY_Q))
        {
            m_CameraRotation -= m_CameraRotationSpeed * time.DeltaTime();
        }
        if (Input::IsKeyPressed(CR_KEY_E))
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
        dispatcher.Dispatch<WindowResizeEvent>(CR_BIND_EVENT_FN(OrthographicCameraController::OnResizeEvent));

    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }


    bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& event)
    {
        m_ZoomLevel += event.GetYOffset() * 0.12f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.2f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnResizeEvent(WindowResizeEvent& event)
    {
        m_AspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        OnResize((float)event.GetWidth(), (float)event.GetHeight());
        return false;
    }
}
