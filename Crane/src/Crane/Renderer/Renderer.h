#pragma once

#include "RenderCommand.h"

#include "Crane/Camera/OrthographicCamera.h"

#include "Shader.h"

namespace Crane
{
    class Renderer
    {
    public:
        static void Init();
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