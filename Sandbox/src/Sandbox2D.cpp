#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/Shader/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1.6f / 0.9f) {}

void Sandbox2D::OnAttach()
{
    m_Texture = Crane::Texture2D::Create("assets/textures/logo.png");
}
void Sandbox2D::OnDetach()

{
}

void Sandbox2D::OnUpdate(Crane::Time time)
{
    CR_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(time);

    Crane::Renderer2D::ResetStats();

    {
        CR_PROFILE_SCOPE("Renderer Prep");

        Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
        Crane::RenderCommand::Clear();
    }

    {
        CR_PROFILE_SCOPE("Renderer Draw");
        Crane::TextureParameters textureParameters(m_Texture);
        textureParameters.Color = m_Color;

        Crane::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Crane::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_Color);
        Crane::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 0.5f, 0.75f }, textureParameters);
        Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, textureParameters);
        Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, textureParameters);
        Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, textureParameters);
        Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, textureParameters);
        Crane::Renderer2D::DrawRotatedQuad({ -0.2f, 0.5f, 0.2f }, m_Angle, m_Scale, textureParameters);
    }

    Crane::Renderer2D::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
    CR_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Triangle color", glm::value_ptr(m_Color));
    ImGui::DragFloat3("Position", glm::value_ptr(m_Position));
    ImGui::SliderAngle("Rotation", &m_Angle);
    ImGui::DragFloat2("Scale", glm::value_ptr(m_Scale));
    ImGui::End();
}
void Sandbox2D::OnEvent(Crane::Event& event)
{
    m_CameraController.OnEvent(event);
}
