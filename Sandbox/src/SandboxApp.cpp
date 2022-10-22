#include <Crane.h>
#include <Crane/Core/EntryPoint.h>

#include "Sandbox2D.h"

namespace Crane {
    class Sandbox : public Application
    {
    public:
        Sandbox(Crane::ApplicationSpecification& specification)
            :Application(specification)
        {
            PushLayer(new Sandbox2D());
        }

        ~Sandbox() {}
    };

    Application* CreateApplication(ApplicationCommandLineArgs args)
    {
        ApplicationSpecification spec;
        spec.Name = "Sandbox";
        spec.WorkingDirectory = "../Sparrow";
        spec.CommandLineArgs = args;
        return new Sandbox(spec);
    }
}