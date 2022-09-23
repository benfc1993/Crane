#pragma once

#include "Crane/Camera/OrthographicCamera.h"
#include "Shader/Texture.h"



namespace Crane {
    struct TextureParameters
    {
        Ref<Texture2D>& Texture;
        float TilingFactor = 1.0f;
        glm::vec4 Color = glm::vec4(1.0f);

        TextureParameters(Ref<Texture2D>& texture) : Texture(texture) {}
    };

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, TextureParameters textureParameters);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, TextureParameters textureParameters);

        static void DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));

        static void DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, TextureParameters textureParameters);
        static void DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, TextureParameters textureParameters);

        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadsDrawn = 0;

            uint32_t GetTotalVertexCount() { return QuadsDrawn * 4; }
            uint32_t GetTotalIndexCount() { return QuadsDrawn * 6; }
        };

        static void ResetStats();
        static Statistics GetStats();
    };
}