#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



#include "Platform/OpenGL/Shader/OpenGLShader.h"

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1.6f / 0.9f) {}

void Sandbox2D::OnAttach()
{
    // --------- Square --------
    m_SquareVertexArray = Crane::VertexArray::Create();

    float squareVertices[4 * 5] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };

    Crane::Ref<Crane::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer = Crane::VertexBuffer::Create(sizeof(squareVertices), squareVertices);

    Crane::BufferLayout layout = {
        {Crane::ShaderDataType::Float3, "a_Position"},
        {Crane::ShaderDataType::Float2, "a_Texture"}
    };

    squareVertexBuffer->SetLayout(layout);

    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

    uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
    Crane::Ref<Crane::IndexBuffer> indexBuffer;
    indexBuffer = Crane::IndexBuffer::Create(sizeof(indices) / sizeof(u_int32_t), indices);

    m_SquareVertexArray->SetIndexBuffer(indexBuffer);

    m_ShaderLibrary.Load("assets/shaders/FlatShader.glsl");
}
void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Crane::Time time)
{
    m_CameraController.OnUpdate(time);

    Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
    Crane::RenderCommand::Clear();

    auto flatShader = m_ShaderLibrary.Get("FlatShader");

    Crane::Renderer::BeginScene(m_CameraController.GetCamera());

    std::dynamic_pointer_cast<Crane::OpenGLShader>(flatShader)->Bind();
    std::dynamic_pointer_cast<Crane::OpenGLShader>(flatShader)->UploadUniformFloat3("u_Color", m_TriangleColor);
    Crane::Renderer::Submit(flatShader, m_SquareVertexArray);

    Crane::Renderer::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Triangle color", glm::value_ptr(m_TriangleColor));
    ImGui::End();
}
void Sandbox2D::OnEvent(Crane::Event& event)
{
    m_CameraController.OnEvent(event);
}
