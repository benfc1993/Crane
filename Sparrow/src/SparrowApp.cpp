#include <Crane.h>
#include <Crane/Core/EntryPoint.h>

#include "Platform/OpenGL/Shader/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Crane {
    class Sparrow: public Application
    {
    public:
        Sparrow(ApplicationSpecification& specification)
            :Application(specification)
        {
            PushLayer(new EditorLayer());
        }

        ~Sparrow() {}
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification spec;
        spec.Name = "Sandbox";
        spec.WorkingDirectory = ".";
        spec.CommandLineArgs = args;

        // Project::New("SandboxProject", "/home/ben/projects/SandboxProject/")->Save("/home/ben/projects/SandboxProject/SandboxProject.crproj");
        Project::Load("/home/ben/projects/SandboxProject/SandboxProject.crproj");
        return new Sparrow(spec);
    }
}
