#include <Crane.h>

#include "imgui/imgui.h"

class ExampleLayer : public Crane::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override
    {
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello World");
        ImGui::End();
    }

    void OnEvent(Crane::Event& event) override
    {
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

Crane::Application* Crane::CreateApplication()
{
    return new Sandbox();
}