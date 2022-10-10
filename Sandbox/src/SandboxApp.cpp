#include <Crane.h>
#include <Crane/Core/EntryPoint.h>

#include "Sandbox2D.h"

// class ExampleLayer : public Crane::Layer
// {
// public:
//     ExampleLayer() : Layer("Example"), m_CameraController(1.6f / 0.9f)
//     {

//         // --------- Square --------
//         m_SquareVertexArray = Crane::VertexArray::Create();

//         float squareVertices[4 * 5] = {
//             -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//             0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
//             -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//         };

//         Crane::Ref<Crane::VertexBuffer> squareVertexBuffer;
//         squareVertexBuffer = Crane::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

//         Crane::BufferLayout layout = {
//             {Crane::ShaderDataType::Float3, "a_Position"},
//             {Crane::ShaderDataType::Float2, "a_TextureCoord"}
//         };

//         squareVertexBuffer->SetLayout(layout);

//         m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

//         uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
//         Crane::Ref<Crane::IndexBuffer> indexBuffer;
//         indexBuffer = Crane::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

//         m_SquareVertexArray->SetIndexBuffer(indexBuffer);

//         // --------- Triangle --------
//         m_TriangleVertexArray = Crane::VertexArray::Create();

//         float triangleVertices[3 * 5] = {
//             -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//             0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
//         };

//         Crane::Ref<Crane::VertexBuffer> triangleVertexBuffer;
//         triangleVertexBuffer = Crane::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));

//         Crane::BufferLayout triangleLayout = {
//             {Crane::ShaderDataType::Float3, "a_Position"},
//             {Crane::ShaderDataType::Float2, "a_TextureCoord"}
//         };

//         triangleVertexBuffer->SetLayout(triangleLayout);

//         m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

//         uint32_t triangleIndices[6] = { 0, 1, 2 };
//         Crane::Ref<Crane::IndexBuffer> triangleIndexBuffer;
//         triangleIndexBuffer = Crane::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));

//         m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

//         m_FlatShader = Crane::Shader::Create("assets/shaders/FlatShader.glsl");
//         auto textureShader = m_ShaderLibrary.Load("assets/shaders/TextureShader.glsl");

//         m_GridTexture = Crane::Texture2D::Create("assets/textures/test.png");
//         m_LogoTexture = Crane::Texture2D::Create("assets/textures/logo.png");

//         std::dynamic_pointer_cast<Crane::OpenGLShader>(textureShader)->Bind();
//         std::dynamic_pointer_cast<Crane::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
//     }

//     void OnUpdate(Crane::Time time) override
//     {
//         m_CameraController.OnUpdate(time);

//         Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
//         Crane::RenderCommand::Clear();

//         auto textureShader = m_ShaderLibrary.Get("TextureShader");

//         Crane::Renderer::BeginScene(m_CameraController.GetCamera());

//         glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), m_SquarePosition);

//         m_GridTexture->Bind();
//         Crane::Renderer::Submit(textureShader, m_SquareVertexArray, squareTransform);

//         m_LogoTexture->Bind();
//         Crane::Renderer::Submit(textureShader, m_SquareVertexArray);

//         std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->Bind();
//         std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_TriangleColor);

//         glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
//         Crane::Renderer::Submit(m_FlatShader, m_TriangleVertexArray, triangleTransform);

//         Crane::Renderer::EndScene();
//     }

//     virtual void OnImGuiRender() override
//     {
//         ImGui::Begin("Settings");
//         ImGui::ColorEdit3("Triangle color", glm::value_ptr(m_TriangleColor));
//         ImGui::End();
//     }

//     void OnEvent(Crane::Event& event) override
//     {
//         m_CameraController.OnEvent(event);
//     }

// private:
//     Crane::ShaderLibrary m_ShaderLibrary;
//     Crane::Ref<Crane::Shader> m_Shader, m_FlatShader;
//     Crane::Ref<Crane::VertexArray> m_SquareVertexArray;
//     Crane::Ref<Crane::VertexArray> m_TriangleVertexArray;
//     Crane::OrthographicCameraController m_CameraController;

//     Crane::Ref<Crane::Texture2D> m_GridTexture, m_LogoTexture;

//     glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
//     float m_CameraSpeed = 2.0f;
//     float m_CameraRotation = 0.0f;
//     float m_CameraRotationSpeed = 100.0f;

//     glm::vec3 m_TriangleColor{ 0.8f, 0.2f, 0.3f };
//     glm::vec3 m_TrianglePosition{ 1.0f, 0.0f, 0.0f };

//     glm::vec3 m_SquarePosition{ -1.1f, 0.0f, 0.0f };
// };

namespace Crane {
    class Sandbox : public Application
    {
    public:
        Sandbox(ApplicationCommandLineArgs args)
            :Application("Sandbox", args)
        {
            PushLayer(new Sandbox2D());
        }

        ~Sandbox() {}
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        return new Sandbox(args);
    }
}