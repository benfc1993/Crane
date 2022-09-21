#pragma once

#include "Crane/Camera/OrthographicCamera.h"
#include "Shader/Texture.h"



namespace Crane {
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const float angle, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const float angle, const glm::vec2& size, Ref<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f));
    };
}