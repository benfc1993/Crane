#include <Crane.h>
#include <Crane/Core/EntryPoint.h>

class ExampleLayer : public Crane::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override
    {
        CR_INFO("ExampleLayer::OnUpdate");
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
    }
    ~Sandbox() {}
};

Crane::Application *Crane::CreateApplication()
{
    return new Sandbox();
}
