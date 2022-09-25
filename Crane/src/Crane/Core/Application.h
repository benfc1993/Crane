#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/Window.h"
#include "Crane/Core/LayerStack.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/ImGui/ImGuiLayer.h"

namespace Crane
{
    class Application
    {
    public:
        Application(const std::string& name = "Crane App");

        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static inline Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);

    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimised = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;

    private:
        static Application* s_Instance;
    };

    Application* CreateApplication();
}