#pragma once

#include "Crane/Core/Layer.h"

#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"
#include "Crane/Events/ApplicationEvent.h"

#include "ImGuiTheme.h"

namespace Crane
{
    class ImGuiLayer : public Layer
    {

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

        void ShouldBlockEvents(bool shouldHandleEvents) { m_ShouldBlockEvents = shouldHandleEvents; }

        void SetDarkThemeColors(ImGuiTheme& theme);

    private:
        bool m_ShouldBlockEvents = true;
        float m_Time = 0.0f;
    };
}