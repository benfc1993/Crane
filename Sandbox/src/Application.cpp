#include <Crane.h>

class ExampleLayer : public Crane::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override
    {
    }

    void OnEvent(Crane::Event &event) override
    {
        CR_TRACE("{0}", event);
    }
};

class Sandbox : public Crane::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new Crane::ImGuiLayer());
    }
    ~Sandbox() {}
};

Crane::Application *Crane::CreateApplication()
{
    return new Sandbox();
}
