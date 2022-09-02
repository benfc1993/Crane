#pragma once

#include "Crane/Core/Base.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

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

#define CR_INFO(...) Crane::Log::GetClientLogger()->info(__VA_ARGS__);
#define CR_TRACE(...) Crane::Log::GetClientLogger()->trace(__VA_ARGS__);
#define CR_WARN(...) Crane::Log::GetClientLogger()->warn(__VA_ARGS__);
#define CR_ERROR(...) Crane::Log::GetClientLogger()->error(__VA_ARGS__);
#define CR_FATAL(...) Crane::Log::GetClientLogger()->fatal(__VA_ARGS__);