#pragma once

#include "imgui.h"

#include "Crane/Core/Layer.h"

#include "Crane/Events/KeyEvent.h"
#include "Crane/Events/MouseEvent.h"
#include "Crane/Events/ApplicationEvent.h"


namespace Crane
{
    class ImGuiLayer : public Layer
    {

    public:
        struct ThemeColors
        {
            ImVec4 Primary{ 0.23f, 0.95f, 1.0f, 1.0f };
            ImVec4 Secondary{ 0.96f, 0.38f, 0.0f, 1.0f };
            ImVec4 White{ 0.98, 1.0f, 1.0f, 1.0f };
            ImVec4 Dark{ 0.22f, 0.28f, 0.36f, 1.0f };
            ImVec4 Medium{ 0.25f, 0.31f, 0.39f, 1.0f };
            ImVec4 Light{ 0.36f, 0.45f, 0.55f, 1.0f };
            ImVec4 WindowBg{ 0.205f, 0.2f, 0.21f, 1.0f };

            // ImVec4 primary = { 0.23f, 0.95f, 1.0f, 1.0f }; //#3BF3FF
            // ImVec4 white = { 0.98, 1.0f, 1.0f, 1.0f }; //#F9FFFF
            // ImVec4 darkGrey = { 0.22f, 0.28f, 0.36f, 1.0f }; //#38475C
            // ImVec4 grey = { 0.25f, 0.31f, 0.39f, 1.0f }; //#414F63
            // ImVec4 lightGrey = { 0.36f, 0.45f, 0.55f, 1.0f }; //#5C738D
        };

        ThemeColors m_Theme;
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

        void SetDarkThemeColors();

    private:
        bool m_ShouldBlockEvents = true;
        float m_Time = 0.0f;
    };
}