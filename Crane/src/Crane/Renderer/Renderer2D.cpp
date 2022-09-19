#include "crpch.h"

#include "Renderer2D.h"
#include "Shader/VertexArray.h"
#include "Shader/Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>



namespace Crane {

    struct Renderer2DData
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
        Ref<Shader> TextureShader;
    };

    static Renderer2DData* s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DData();

        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[4 * 5] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        };

        Ref<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer = VertexBuffer::Create(sizeof(squareVertices), squareVertices);

        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_Texture"}
        };

        squareVertexBuffer->SetLayout(layout);

        s_Data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);

        uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> indexBuffer;
        indexBuffer = IndexBuffer::Create(sizeof(indices) / sizeof(u_int32_t), indices);

        s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

        s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatShader.glsl");
        s_Data->TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }
    void Renderer2D::Shutdown()
    {
        delete s_Data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());

        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
    }
    void Renderer2D::EndScene()
    {

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, angle, size, color);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, glm::vec4& color)
    {
        s_Data->FlatColorShader->Bind();
        s_Data->FlatColorShader->SetFloat4("u_Color", color);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->FlatColorShader->SetMat4("u_Transform", transform);

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, angle, size, texture, color);
    }
    void Renderer2D::DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color)
    {
        s_Data->TextureShader->Bind();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        s_Data->TextureShader->SetMat4("u_Transform", transform);
        s_Data->TextureShader->SetFloat4("u_Color", color);

        texture->Bind();

        s_Data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
    }
}

