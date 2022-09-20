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


    glm::vec4 m_Color{ 0.8f, 0.2f, 0.3f, 1.0f };
    glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
    glm::vec2 m_Scale{ 1.0f, 1.0f };
    float m_Angle = 0.0f;
    Crane::Ref<Crane::VertexArray> m_SquareVertexArray;
    Crane::Ref<Crane::VertexArray> m_TriangleVertexArray;

    Crane::ParticleData m_Particle;
    Crane::ParticleSystem m_ParticleSystem;
    int m_ParticleBurstSize = 5;
};
