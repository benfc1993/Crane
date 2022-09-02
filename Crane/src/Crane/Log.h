#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Crane
{

    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

#define CR_CORE_INFO(...) Crane::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CR_CORE_TRACE(...) Crane::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CR_CORE_WARN(...) Crane::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CR_CORE_ERROR(...) Crane::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CR_CORE_FATAL(...) Crane::Log::GetCoreLogger()->fatal(__VA_ARGS__);

#define CR_INFO(...) Crane::Log::GetCoreLogger()->info(__VA_ARGS__);
#define CR_TRACE(...) Crane::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define CR_WARN(...) Crane::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define CR_ERROR(...) Crane::Log::GetCoreLogger()->error(__VA_ARGS__);
#define CR_FATAL(...) Crane::Log::GetCoreLogger()->fatal(__VA_ARGS__);