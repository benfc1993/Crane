#include "crpch.h"

#include "Application.h"

#include "Crane/Log.h"

#include <GLFW/glfw3.h>

namespace Crane
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }
    Application::~Application() {}

    void Application::Run()
    {
        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }
}