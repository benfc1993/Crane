#include "crpch.h"
#include "Framebuffer.h"

#include "Crane/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Crane
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& specification)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            CR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLFramebuffer>(specification);
        }

        CR_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}