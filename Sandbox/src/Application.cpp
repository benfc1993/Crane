#include <Crane.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Crane::Layer
{
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) {
        m_VertexArray.reset(Crane::VertexArray::Create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.13f, 0.3f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.3f, 0.8f, 0.13f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.13f, 0.3f, 0.8f, 1.0f,
        };

        Crane::Ref<Crane::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Crane::VertexBuffer::Create(sizeof(vertices), vertices));

        Crane::BufferLayout layout = {
            {Crane::ShaderDataType::Float3, "a_Position"},
            {Crane::ShaderDataType::Float4, "a_Color"}
        };

        vertexBuffer->SetLayout(layout);

        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = { 0, 1, 2 };
        Crane::Ref<Crane::IndexBuffer> indexBuffer;
        indexBuffer.reset(Crane::IndexBuffer::Create(sizeof(indices) / sizeof(u_int32_t), indices));

        m_VertexArray->SetIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ProjectionView;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ProjectionView * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5 , 1.0);
                color = v_Color;
            }
        )";
        m_Shader.reset(Crane::Shader::Create(vertexSrc, fragmentSrc));

        std::string flatColorShader = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            uniform vec3 u_Color;

            void main()
            {
                color = vec4(u_Color, 1.0f);
            }
        )";
        m_FlatShader.reset(Crane::Shader::Create(vertexSrc, flatColorShader));

    }

    void OnUpdate(Crane::Time time) override
    {
        OnKeyPressedEvent(time.DeltaTime());

        CR_TRACE("Current time: {0} s, {1} ms", time.GetTime(), time.GetMilliseconds());
        CR_TRACE("Delta time: {0} s", time.DeltaTime());

        Crane::RenderCommand::SetClearColor(glm::vec4(0.1333f, 0.1333f, 0.1333f, 1));
        Crane::RenderCommand::Clear();


        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Crane::Renderer::BeginScene(m_Camera);

        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->Bind();
        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_FlatShader)->UploadUniformFloat3("u_Color", m_TriangleColor);

        glm::mat4 triangleTransform = glm::translate(glm::mat4(1.0f), m_TrianglePosition);
        Crane::Renderer::Submit(m_FlatShader, m_VertexArray, triangleTransform);

        std::dynamic_pointer_cast<Crane::OpenGLShader>(m_Shader)->Bind();
        Crane::Renderer::Submit(m_Shader, m_VertexArray);

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
    Crane::Ref<Crane::Shader> m_Shader;
    Crane::Ref<Crane::Shader> m_FlatShader;
    Crane::Ref<Crane::VertexArray> m_VertexArray;
    Crane::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition{ 0.0f, 0.0f, 0.0f };
    float m_CameraSpeed = 2.0f;
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 100.0f;

    glm::vec3 m_TriangleColor{ 0.8f, 0.2f, 0.3f };
    glm::vec3 m_TrianglePosition{ -0.75f, 0.0f, 0.0f };
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