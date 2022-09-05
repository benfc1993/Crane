#include "crpch.h"

#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Crane
{

    VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexBuffer(size, vertices);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t size, uint32_t* indices)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLIndexBuffer(size, indices);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}