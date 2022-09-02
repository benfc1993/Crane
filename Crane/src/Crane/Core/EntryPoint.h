#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/Application.h"

extern Crane::Application *Crane::CreateApplication();

int main(int argc, char **argv)
{
    Crane::Log::Init();
    CR_CORE_WARN("Initialised log");
    CR_INFO("Hello! ");

    auto app = Crane::CreateApplication();
    app->Run();
    delete app;
}