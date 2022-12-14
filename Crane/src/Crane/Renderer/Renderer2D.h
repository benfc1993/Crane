#pragma once

#include "Crane/Camera/OrthographicCamera.h"
#include "Crane/Camera/Camera.h"
#include "Crane/Camera/EditorCamera.h"
#include "Crane/Scene/Components.h"

#include "Shader/Texture.h"

namespace Crane {
    struct TextureParameters
    {
        Ref<Texture2D> Texture;
        float TilingFactor = 1.0f;
        glm::vec4 Color = glm::vec4(1.0f);

        TextureParameters() = default;
        TextureParameters(Ref<Texture2D>& texture) : Texture(texture) {}
    };

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityId = -1);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId = -1);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture, int entityId = -1);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture, int entityId = -1);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, TextureParameters& textureParameters, int entityId = -1);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, TextureParameters& textureParameters, int entityId = -1);

        static void DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);
        static void DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);

        static void DrawRotatedQuad(const glm::vec2& position, const float degrees, const glm::vec2& size, TextureParameters& textureParameters, int entityId = -1);
        static void DrawRotatedQuad(const glm::vec3& position, const float degrees, const glm::vec2& size, TextureParameters& textureParameters, int entityId = -1);

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
        static void DrawQuad(const glm::mat4& transform, TextureParameters& textureParameters, int entityId = -1);

        static void DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.0f, float fade = 0.005f, int entityId = -1);

        static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityId = -1);

        static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, int entityId = -1);
        static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityId = -1);
        static void DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);

        static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityId);

        static float GetLineWidth();
        static void SetLineWidth(float lineWidth);

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