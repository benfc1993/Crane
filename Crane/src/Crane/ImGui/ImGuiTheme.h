#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Crane {
    struct ImGuiTheme
    {
        std::string Name = "Default";
        std::string FilePath;
        glm::vec4 Primary{ 0.226f, 0.721f, 0.755f, 1.000f };  //#3BF3FF
        glm::vec4 PrimaryLight{ 0.117f, 0.817f, 0.869f, 1.000f }; //#00EEFF
        glm::vec4 PrimaryDark{ 0.218f, 0.524f, 0.549f, 1.000f }; //#77F5FF
        glm::vec4 Secondary{ 0.96f, 0.38f, 0.0f, 1.0f };
        glm::vec4 White{ 0.98, 1.0f, 1.0f, 1.0f }; //#F9FFFF
        glm::vec4 Dark{ 0.150f, 0.167f, 0.190f, 1.000f }; //#38475C
        glm::vec4 Medium{ 0.167f, 0.197f, 0.236f, 1.000f }; //#414F63
        glm::vec4 Light{ 0.289f, 0.322f, 0.359f, 1.000f }; //#5C738D
        glm::vec4 WindowBg{ 0.135f, 0.132f, 0.139f, 1.000f }; //#A87779
    };
}