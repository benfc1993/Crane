#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/Application.h"

extern Crane::Application* Crane::CreateApplication();

int main(int argc, char** argv)
{
    Crane::Log::Init();
    CR_CORE_WARN("Initialised log");

    CR_BEGIN_PROFILE_SESSION("Startup", "CraneProfile-Startup.json");
    auto app = Crane::CreateApplication();
    CR_END_PROFILE_SESSION();

    CR_BEGIN_PROFILE_SESSION("Runtime", "CraneProfile-Runtime.json");
    app->Run();
    CR_END_PROFILE_SESSION();

    CR_BEGIN_PROFILE_SESSION("Shutdown", "CraneProfile-Shutdown.json");
    delete app;
    CR_END_PROFILE_SESSION();

}