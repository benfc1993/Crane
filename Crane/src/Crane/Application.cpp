#include "crpch.h"

#include "Application.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/Log.h"

namespace Crane
{
    Application::Application() {}
    Application::~Application() {}

    void Application::Run()
    {
        WindowResizeEvent e(1200, 720);
        CR_TRACE(e);
        while (true)
            ;
    }
}