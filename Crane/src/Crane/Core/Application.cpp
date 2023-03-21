#include "crpch.h"
#include "Application.h"

#include "Crane/Core/Log.h"

#include "Crane/Renderer/Renderer.h"
#include "Crane/Scripting/ScriptEngine.h"

#include <GLFW/glfw3.h>

namespace Crane
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		CR_PROFILE_FUNCTION();

		CR_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(m_Specification.Name)));
		m_Window->SetEventCallback(CR_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		CR_PROFILE_FUNCTION();

		ScriptEngine::Shutdown();
		Renderer::Shutdown();
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

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{

			ExecuteMainThreadQueue();

			float frameTime = (float)glfwGetTime();
			Time time((float)frameTime, (float)frameTime - m_LastFrameTime);
			m_LastFrameDuration = (float)frameTime - m_LastFrameTime;
			m_LastFrameTime = frameTime;

			if (!m_Minimised)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(time);

				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}


			m_Window->OnUpdate();
		}
	}

	void Application::SubmitToMainThread(std::function<void()> func)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		m_MainThreadQueue.emplace_back(func);
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
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