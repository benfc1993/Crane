#pragma once

#include "Core.h"
#include "Window.h"

#include "Crane/Events/ApplicationEvent.h"

namespace Crane
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void OnEvent(Event &e);

    private:
        bool OnWindowClose(WindowCloseEvent &e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

    Application *CreateApplication();
}