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

        int EntityId = 0;
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

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

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

        s_Data.QuadVertexBuffer->SetLayout({ {ShaderDataType::Float3, "a_Position"},
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

        s_Data.WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_Data.MaxTextureSlots];

        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
            samplers[i] = i;

        s_Data.TextureShader = Shader::Create("assets/shaders/TextureShader.glsl");


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

        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

        Flush();
    }

    void Renderer2D::Flush()
    {
        if (s_Data.QuadIndexCount)
        {
            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
            {
                s_Data.TextureSlots[i]->Bind(i);
            }

            s_Data.TextureShader->Bind();
            RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
            // ResetRenderData();
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

    void Renderer2D::DrawQuad(glm::mat4& transform, const glm::vec4& color, int entityId)
    {
        CheckDrawCall();

        const float textureIndex = 0.0f;
        const float tilingFactor = 1.0f;

        s_Data.WhiteTexture->Bind();

        AddQuadToVertexBuffer(color, transform, textureIndex, tilingFactor, entityId);
    }
    void Renderer2D::DrawQuad(glm::mat4& transform, TextureParameters& textureParameters, int entityId)
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

    void Renderer2D::DrawSprite(glm::mat4& transform, SpriteRendererComponent& src, int entityId)
    {
        DrawQuad(transform, src.Color, entityId);
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
