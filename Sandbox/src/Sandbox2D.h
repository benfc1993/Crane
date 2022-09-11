#pragma once

#include <Crane.h>

class Sandbox2D : public Crane::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Crane::Time time) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Crane::Event& event) override;
private:
    Crane::OrthographicCameraController m_CameraController;

    Crane::Ref<Crane::Shader> m_Shader;
    Crane::Ref<Crane::Texture2D> m_Texture;


    glm::vec3 m_TriangleColor{ 0.8f, 0.2f, 0.3f };
    Crane::Ref<Crane::VertexArray> m_SquareVertexArray;
    Crane::Ref<Crane::VertexArray> m_TriangleVertexArray;

    Crane::ShaderLibrary m_ShaderLibrary;
    Crane::Ref<Crane::Texture2D> m_GridTexture, m_LogoTexture;
};
