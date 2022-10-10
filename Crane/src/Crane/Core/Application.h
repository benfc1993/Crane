#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/Window.h"
#include "Crane/Core/LayerStack.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/ImGui/ImGuiLayer.h"

namespace Crane
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            CR_CORE_ASSERT(index < Count, "");
            return Args[index];
        }
    };

    class Application
    {
    public:
        Application(const std::string& name = "Sparrow App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
        virtual ~Application();

        void Run();
        void Close();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        static inline Application& Get() { return *s_Instance; }
        inline Window& GetWindow() { return *m_Window; }

        float GetLastFrameDuration() { return m_LastFrameDuration; }

        ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnWindowMinimize(WindowMinimizeEvent& e);

    private:
        ApplicationCommandLineArgs m_CommandLineArgs;
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimised = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
        float m_LastFrameDuration = 0.0f;

    private:
        static Application* s_Instance;
    };

    Application* CreateApplication(ApplicationCommandLineArgs args);
}