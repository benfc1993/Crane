#pragma once

namespace Crane {
    class ScriptEngine
    {
    public:
        ScriptEngine() = default;
        static void Init();
        static void Shutdown();
    private:
        static void InitMono();
        static void ShutdownMono();
    };
}