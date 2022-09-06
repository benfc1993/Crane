#include "crpch.h"

#include "Renderer.h"

namespace Crane
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->projectionViewMatrix = camera.GetProjectionViewMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray>& vertexArray, glm::mat4 transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ProjectionView", m_SceneData->projectionViewMatrix);
        shader->UploadUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}
