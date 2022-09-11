#pragma once

#include "RenderCommand.h"

#include "Crane/Camera/OrthographicCamera.h"

#include "Crane/Renderer/Shader/Shader.h"

namespace Crane
{
    class Renderer
    {
    public:
        static void Init();
        static void OnWindowResize(uint32_t width, uint32_t height);
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader, Ref<VertexArray>& vertexArray, glm::mat4 transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 projectionViewMatrix;
        };

        static SceneData* m_SceneData;
    };

}