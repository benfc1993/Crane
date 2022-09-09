#include <Crane.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Crane::Layer
{
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {

        // --------- Square --------
        m_SquareVertexArray.reset(Crane::VertexArray::Create());

        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };

        Crane::Ref<Crane::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(Crane::VertexBuffer::Create(sizeof(squareVertices), squareVertices));

        Crane::BufferLayout layout = {
            {Crane::ShaderDataType::Float3, "a_Position"},
            {Crane::ShaderDataType::Float2, "a_Texture"}
        };

        squareVertexBuffer->SetLayout(layout);

        m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
        Crane::Ref<Crane::IndexBuffer> indexBuffer;
        indexBuffer.reset(Crane::IndexBuffer::Create(sizeof(indices) / sizeof(u_int32_t), indices));

        m_SquareVertexArray->SetIndexBuffer(indexBuffer);

        // --------- Triangle --------
        m_TriangleVertexArray.reset(Crane::VertexArray::Create());

        float triangleVertices[3 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
        };

        Crane::Ref<Crane::VertexBuffer> triangleVertexBuffer;
        triangleVertexBuffer.reset(Crane::VertexBuffer::Create(sizeof(triangleVertices), triangleVertices));

        Crane::BufferLayout triangleLayout = {
            {Crane::ShaderDataType::Float3, "a_Position"},
            {Crane::ShaderDataType::Float2, "a_Texture"}
        };

        triangleVertexBuffer->SetLayout(triangleLayout);

        m_TriangleVertexArray->AddVertexBuffer(triangleVertexBuffer);

        uint32_t triangleIndices[6] = { 0, 1, 2 };
        Crane::Ref<Crane::IndexBuffer> triangleIndexBuffer;
        triangleIndexBuffer.reset(Crane::IndexBuffer::Create(sizeof(triangleIndices) / sizeof(u_int32_t), triangleIndices));

        m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

        m_FlatShader.reset(Crane::Shader::Create("assets/shaders/FlatShader.glsl"));
        m_TextureShader.reset(Crane::Shader::Create("assets/shaders/TextureShader.glsl"));

        m_Texture = Crane::Texture2D::Create("assets/textures/test.png");
        m_AlphaTexture = Crane::Texture2D::Create("assets/textures/logo.png");

        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Crane::Time time) override
    {
        OnKeyPressedEvent(time.DeltaTime());

        Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
        Crane::RenderCommand::Clear();


        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Crane::Renderer::BeginScene(m_Camera);

        glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), m_SquarePosition);

        m_Texture->Bind();
        Crane::Renderer::Submit(m_TextureShader, m_SquareVertexArray, squareTransform);

        m_AlphaTexture->Bind();
        Crane::Renderer::Submit(m_TextureShader, m_SquareVertexArray);

        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->Bind();
        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_TriangleColor);

        glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
        Crane::Renderer::Submit(m_FlatShader, m_TriangleVertexArray, triangleTransform);

        Crane::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Triangle color", glm::value_ptr(m_TriangleColor));
        ImGui::End();
    }

    void OnEvent(Crane::Event& event) override
    {
    }

    void OnKeyPressedEvent(float deltaTime)
    {

        if (Crane::Input::IsKeyPressed(CR_KEY_LEFT))
        {
            m_CameraPosition.x -= m_CameraSpeed * deltaTime;
        }
        if (Crane::Input::IsKeyPressed(CR_KEY_RIGHT))
        {
            m_CameraPosition.x += m_CameraSpeed * deltaTime;
        }
        if (Crane::Input::IsKeyPressed(CR_KEY_UP))
        {
            m_CameraPosition.y += m_CameraSpeed * deltaTime;
        }
        if (Crane::Input::IsKeyPressed(CR_KEY_DOWN))
        {
            m_CameraPosition.y -= m_CameraSpeed * deltaTime;
        }
        if (Crane::Input::IsKeyPressed(CR_KEY_Q))
        {
            m_CameraRotation -= m_CameraRotationSpeed * deltaTime;
        }
        if (Crane::Input::IsKeyPressed(CR_KEY_E))
        {
            m_CameraRotation += m_CameraRotationSpeed * deltaTime;
        }
    }
private:
    Crane::Ref<Crane::Shader> m_Shader, m_FlatShader, m_TextureShader;
    Crane::Ref<Crane::VertexArray> m_SquareVertexArray;
    Crane::Ref<Crane::VertexArray> m_TriangleVertexArray;
    Crane::OrthographicCamera m_Camera;

    Crane::Ref<Crane::Texture2D> m_Texture, m_AlphaTexture;

    glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
    float m_CameraSpeed = 2.0f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 100.0f;

    glm::vec3 m_TriangleColor{ 0.8f, 0.2f, 0.3f };
    glm::vec3 m_TrianglePosition{ 1.0f, 0.0f, 0.0f };

    glm::vec3 m_SquarePosition{ -1.1f, 0.0f, 0.0f };
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