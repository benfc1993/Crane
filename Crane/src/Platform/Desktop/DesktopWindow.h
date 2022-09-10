#pragma once

#include "Crane/Core/Window.h"

#include "Crane/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Crane
{

    class DesktopWindow : public Window
    {
    public:
        DesktopWindow(const WindowProps& props);
        virtual ~DesktopWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        bool IsMinimized() const override { return m_Data.IsMinimized; };

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            bool IsMinimized;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}