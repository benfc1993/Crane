#include "crpch.h"

#include "Buffer.h"

#include "Crane/Renderer/Renderer.h"

#include "Platform/OpenGL/Shader/OpenGLBuffer.h"

namespace Crane
{

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}