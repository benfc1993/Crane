#pragma once

#include "Crane/Core/Layer.h"

#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"
#include "Crane/Events/ApplicationEvent.h"

namespace Crane
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event &event);

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent &e);
        bool OnMouseButtonReleaseEvent(MouseButtonReleasedEvent &e);
        bool OnMouseMovedEvent(MouseMovedEvent &e);
        bool OnMouseScrolledEvent(MouseScrolledEvent &e);
        bool OnKeyPressedEvent(KeyPressedEvent &e);
        bool OnKeyReleasedEvent(KeyReleasedEvent &e);
        bool OnKeyTypedEvent(KeyTypedEvent &e);
        bool OnWindowResizedEvent(WindowResizeEvent &e);

    private:
        float m_Time = 0.0f;
    };
}