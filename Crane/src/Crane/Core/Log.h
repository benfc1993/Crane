#pragma once

#include "Crane/Core/Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
// #include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace Crane
{

    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
        static std::shared_ptr<spdlog::logger>& GetScriptLogger() { return s_ScriptLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::shared_ptr<spdlog::logger> s_ScriptLogger;
    };

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
    return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
    return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
    return os << glm::to_string(quaternion);
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

#define CR_SCR_INFO(...) Crane::Log::GetScriptLogger()->info(__VA_ARGS__);
#define CR_SCR_TRACE(...) Crane::Log::GetScriptLogger()->trace(__VA_ARGS__);
#define CR_SCR_WARN(...) Crane::Log::GetScriptLogger()->warn(__VA_ARGS__);
#define CR_SCR_ERROR(...) Crane::Log::GetScriptLogger()->error(__VA_ARGS__);
#define CR_SCR_FATAL(...) Crane::Log::GetScriptLogger()->fatal(__VA_ARGS__);