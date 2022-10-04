#pragma once

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

namespace Crane {
    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
        return out;
    }

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    // template <typename T, int U>
    // inline YAML::Emitter& operator<<(YAML::Emitter& out, const std::array<T, U>& arr)
    // {
    //     out << YAML::Flow;
    //     out << YAML::BeginSeq;
    //     for (int i = 0; i < U; i++)
    //         out << arr[i];

    //     YAML::EndSeq;
    //     return out;
    // }
}

