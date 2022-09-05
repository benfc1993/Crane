#include "crpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Crane {
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}



