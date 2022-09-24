#include "crpch.h"
#include "Crane/Core/Application.h"

#include "Crane/Core/Log.h"

#include "Crane/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Crane
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const std::string& name)
    {
        CR_PROFILE_FUNCTION();

        CR_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
        m_Window->SetEventCallback(CR_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        CR_PROFILE_FUNCTION();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushLayer(overlay);
        overlay->OnAttach();
    }

    void Application::Close()
    {
        m_Running = false;
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(CR_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(CR_BIND_EVENT_FN(Application::OnWindowResize));
        dispatcher.Dispatch<WindowMinimizeEvent>(CR_BIND_EVENT_FN(Application::OnWindowMinimize));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();
            Time timestep((float)time, (float)time - m_LastFrameTime);
            m_LastFrameTime = time;

            if (!m_Minimised)
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(timestep);

                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
                m_ImGuiLayer->End();
            }


            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowMinimize(WindowMinimizeEvent& e)
    {
        m_Minimised = e.IsMinimized();
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (m_Minimised)
            return false;

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}