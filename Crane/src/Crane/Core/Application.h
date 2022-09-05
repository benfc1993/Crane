#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/Window.h"
#include "Crane/Core/LayerStack.h"
#include "Crane/Events/ApplicationEvent.h"
#include "Crane/ImGui/ImGuiLayer.h"
#include "Crane/Renderer/Shader.h"
#include "Crane/Renderer/Buffer.h"

namespace Crane
{
    class Application
    {
    public:
        Application();

        virtual ~Application();

        void Run();

        void OnEvent(Event &e);

        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);

        static inline Application &Get() { return *s_Instance; }
        inline Window &GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent &e);

        std::unique_ptr<Window> m_Window;
        ImGuiLayer *m_ImGuiLayer;
        bool m_Running = true;
        LayerStack m_LayerStack;

        unsigned int m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

    private:
        static Application *s_Instance;
    };

    Application *CreateApplication();
}