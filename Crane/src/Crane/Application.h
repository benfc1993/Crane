#pragma once

namespace Crane
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        void Run();
    };

    Application *CreateApplication();
}