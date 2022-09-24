#include <Crane.h>
#include <Crane/Core/EntryPoint.h>

#include "Platform/OpenGL/Shader/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Crane {
    class Sparrow : public Application
    {
    public:
        Sparrow()
            :Application("Sparrow")
        {
            PushLayer(new EditorLayer());
        }

        ~Sparrow() {}
    };

    Application* CreateApplication()
    {
        return new Sparrow();
    }
}
