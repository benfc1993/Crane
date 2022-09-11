#include "crpch.h"
#include "VertexArray.h"

#include "Crane/Renderer/Renderer.h"

#include "Platform/OpenGL/Shader/OpenGLVertexArray.h"

namespace Crane {
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}



