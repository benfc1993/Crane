#include "crpch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Crane
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
    }

    void OpenGLContext::Init()
    {
        CR_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CR_CORE_ASSERT(status, "Failed to initialize GLAD");

        CR_CORE_INFO("OpenGL Info:");
        CR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        CR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        CR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
    }

    void OpenGLContext::SwapBuffers()
    {
        CR_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }
}