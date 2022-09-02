#pragma once

#include "Core.h"

extern Crane::Application *Crane::CreateApplication();

int main(int argc, char **argv)
{
    Crane::Log::Init();
    Crane::Log::GetCoreLogger()->warn("Initialised log");
    Crane::Log::GetClientLogger()->info("Hello!");

    auto app = Crane::CreateApplication();
    app->Run();
    delete app;
}