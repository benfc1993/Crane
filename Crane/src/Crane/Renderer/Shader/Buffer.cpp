#include "crpch.h"

#include "Buffer.h"

#include "Crane/Renderer/Renderer.h"

#include "Platform/OpenGL/Shader/OpenGLBuffer.h"

namespace Crane
{

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size, vertices);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t* indices)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(size, indices);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}