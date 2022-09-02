#pragma once

#include "Core.h"

extern Crane::Application *Crane::CreateApplication();

int main(int argc, char **argv)
{
    Crane::Log::Init();
    CR_CORE_LOG_WARN("Initialised log");
    CR_LOG_INFO("Hello! ");

    auto app = Crane::CreateApplication();
    app->Run();
    delete app;
}