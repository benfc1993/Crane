#include "crpch.h"

#include "Renderer2D.h"
#include "Shader/VertexArray.h"
#include "Shader/Shader.h"
#include "Crane/Renderer/UniformBuffer.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Crane
{

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoord;
        float TextureIndex;
        float TilingFactor;

        int EntityId;
    };

    struct CircleVertex
    {
        glm::vec3 WorldPosition;
        glm::vec3 LocalPosition;
        glm::vec4 Color;
        float Thickness;
        float Fade;

        int EntityId;
    };

    struct LineVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;

        int EntityId;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 10000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        Ref<VertexArray> CircleVertexArray;
        Ref<VertexBuffer> CircleVertexBuffer;
        Ref<Shader> CircleShader;

        float LineWidth = 2.0f;
        Ref<VertexArray> LineVertexArray;
        Ref<VertexBuffer> LineVertexBuffer;
        Ref<Shader> LineShader;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t CircleIndexCount = 0;
        CircleVertex* CircleVertexBufferBase = nullptr;
        CircleVertex* CircleVertexBufferPtr = nullptr;

        uint32_t LineVertexCount = 0;
        LineVertex* LineVertexBufferBase = nullptr;
        LineVertex* LineVertexBufferPtr = nullptr;

        glm::vec4 QuadVertexPositions[4];
        glm::vec2 QuadTextureCoords[4];

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        Renderer2D::Statistics Stats;

        struct CameraData
        {
            glm::mat4 ViewProjection;
        };
        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;
    };

    static Renderer2DData s_Data;

    void Renderer2D::Init()
    {
        CR_PROFILE_FUNCTION();

        s_Data.QuadVertexArray = VertexArray::Create();

        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

        s_Data.QuadVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TextureCoord"},
            {ShaderDataType::Float, "a_TextureIndex"},
            {ShaderDataType::Float, "a_TilingFactor"},
            {ShaderDataType::Int, "a_EntityId"}
            });

        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        //Circles
        s_Data.CircleVertexArray = VertexArray::Create();

        s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));

        s_Data.CircleVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_WorldPosition"},
            {ShaderDataType::Float3, "a_LocalPosition"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float, "a_Thickness"},
            {ShaderDataType::Float, "a_Fade"},
            {ShaderDataType::Int, "a_EntityId"}
            });

        s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
        s_Data.CircleVertexArray->SetIndexBuffer(quadIB);
        s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

        //Lines
        s_Data.LineVertexArray = VertexArray::Create();

        s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));

        s_Data.LineVertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Int, "a_EntityId"}
            });

        s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
        s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

        s_Data.WhiteTexture = Texture2D::Create(1, 1);

        int32_t samplers[s_Data.MaxTextureSlots];

        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
        s_Data.CircleShader = Shader::Create("assets/shaders/Renderer2D_Circle.glsl");
        s_Data.LineShader = Shader::Create("assets/shaders/Renderer2D_Line.glsl");


        s_Data.TextureSlots[0] = s_Data.WhiteTexture;

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };

        s_Data.QuadTextureCoords[0] = { 0.0f, 0.0f };
        s_Data.QuadTextureCoords[1] = { 1.0f, 0.0f };
        s_Data.QuadTextureCoords[2] = { 1.0f, 1.0f };
        s_Data.QuadTextureCoords[3] = { 0.0f, 1.0f };

        s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);
    }
    void Renderer2D::Shutdown()
    {
        CR_PROFILE_FUNCTION();
    }

    void ResetRenderData()
    {
        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.CircleIndexCount = 0;
        s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

        s_Data.LineVertexCount = 0;
        s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;


        s_Data.TextureSlotIndex = 1;
    }

    void CheckDrawCall()
    {
        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            Renderer2D::EndScene();
        }
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

        s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        ResetRenderData();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 viewProjection = camera.GetViewProjection();

        s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
        s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

        ResetRenderData();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        CR_PROFILE_FUNCTION();

        s_Data.TextureShader->Bind();
        s_Data.TextureShader->SetMat4("u_ProjectionView", camera.GetProjectionViewMatrix());

        ResetRenderData();
    }
    void Renderer2D::EndScene()
    {
        CR_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount)
        {
            uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);
            }

            s_Data.TextureShader->Bind();
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            s_Data.Stats.DrawCalls++;
        }

        if (s_Data.CircleIndexCount)
        {
            uint32_t dataSize = (uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase;
            s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

            s_Data.CircleShader->Bind();
            RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
            s_Data.Stats.DrawCalls++;
        }

        if (s_Data.LineVertexCount)
        {
            uint32_t dataSize = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase;
            s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

            s_Data.LineShader->Bind();
            RenderCommand::SetLineWidth(s_Data.LineWidth);
            RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
            s_Data.Stats.DrawCalls++;
        }
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color, entityId);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, entityId);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, int entityId)
    {
        TextureParameters textureParameters(texture);

        DrawQuad({ position.x, position.y, 0.0f }, size, textureParameters, entityId);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, int entityId)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        TextureParameters textureParameters(texture);

        DrawQuad(transform, textureParameters, entityId);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, TextureParameters& textureParameters, int entityId)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, textureParameters, entityId);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, TextureParameters& textureParameters, int entityId)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, textureParameters, entityId);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, degrees, size, color, entityId);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), degrees, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, entityId);
    }

    void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, TextureParameters& textureParameters, int entityId)
    {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, degrees, size, textureParameters, entityId);
    }
    void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, TextureParameters& textureParameters, int entityId)
    {
        CR_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(degrees), { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, textureParameters, entityId);
    }

    static void AddQuadToVertexBuffer(const glm::vec4 color, const glm::mat4& transform, float textureIndex, float tilingFactor, int entityId)
    {

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TextureCoord = s_Data.QuadTextureCoords[i];
            s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            s_Data.QuadVertexBufferPtr->EntityId = entityId;
            s_Data.QuadVertexBufferPtr++;
        }

        s_Data.QuadIndexCount += 6;

        s_Data.Stats.QuadsDrawn++;
    }

    static void AddCircleToVertexBuffer(const glm::mat4& transform, const glm::vec4 color, float thickness, float fade, int entityId)
    {

        for (uint32_t i = 0; i < 4; i++)
        {
            s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
            s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
            s_Data.CircleVertexBufferPtr->Color = color;
            s_Data.CircleVertexBufferPtr->Thickness = thickness;
            s_Data.CircleVertexBufferPtr->Fade = fade;
            s_Data.CircleVertexBufferPtr->EntityId = entityId;
            s_Data.CircleVertexBufferPtr++;
        }

        s_Data.CircleIndexCount += 6;

        s_Data.Stats.QuadsDrawn++;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
    {
        CheckDrawCall();

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        s_Data.WhiteTexture->Bind();

        AddQuadToVertexBuffer(color, transform, textureIndex, tilingFactor, entityId);
    }
    void Renderer2D::DrawQuad(const glm::mat4& transform, TextureParameters& textureParameters, int entityId)
    {
        CheckDrawCall();

        const glm::vec4 color = textureParameters.Color;

        float textureIndex = 0.0f;
        const float tilingFactor = textureParameters.TilingFactor;

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i].get() == *textureParameters.Texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureSlotIndex;
            s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureParameters.Texture;
            s_Data.TextureSlotIndex++;
        }

        AddQuadToVertexBuffer(textureParameters.Color, transform, textureIndex, tilingFactor, entityId);
    }

    void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityId)
    {
        AddCircleToVertexBuffer(transform, color, thickness, fade, entityId);
    }

    void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityId)
    {
        s_Data.LineVertexBufferPtr->Position = p0;
        s_Data.LineVertexBufferPtr->Color = color;
        s_Data.LineVertexBufferPtr->EntityId = entityId;
        s_Data.LineVertexBufferPtr++;

        s_Data.LineVertexBufferPtr->Position = p1;
        s_Data.LineVertexBufferPtr->Color = color;
        s_Data.LineVertexBufferPtr->EntityId = entityId;
        s_Data.LineVertexBufferPtr++;

        s_Data.LineVertexCount += 2;
    }

    void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        DrawRect({ position.x, position.y, 0.0f }, size, color, entityId);
    }
    void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        DrawRect(transform, color, entityId);

    }
    void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId)
    {
        for (int i = 0; i < 4; i++)
        {
            auto p0 = transform * s_Data.QuadVertexPositions[i];
            auto p1 = transform * s_Data.QuadVertexPositions[(i + 1) % 4];

            DrawLine(p0, p1, color);
        }
    }


    void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId)
    {
        if ((std::string)*src.Texture.get() != "")
        {
            TextureParameters textureParameters;
            textureParameters.Color = src.Color;
            textureParameters.Texture = src.Texture;

            DrawQuad(transform, textureParameters, entityId);
        }
        else
        {
            DrawQuad(transform, src.Color, entityId);
        }

    }

    float Renderer2D::GetLineWidth()
    {
        return s_Data.LineWidth;
    }

    void Renderer2D::SetLineWidth(float lineWidth)
    {
        s_Data.LineWidth = lineWidth;
    }


    void Renderer2D::ResetStats()
    {
        memset(&s_Data.Stats, 0, sizeof(Statistics));
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_Data.Stats;
    }
}
