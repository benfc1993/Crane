#pragma once

#include <glm/glm.hpp>

#include <imgui/imgui.h>

#include <string>

namespace Crane {
    namespace Drawers {
        void Vector3(const std::string& label, glm::vec3& vector, float resetValue = 0.0f, float columnWidth = 20.0f);
    }

}