#pragma once

#include "Crane/Core/Base.h"
#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
    class ThemeSerializer
    {
    public:
        ThemeSerializer(ImGuiTheme& theme);

        void Serialize(const std::string filePath, const std::string name = "Default");

        bool Deserialize(const std::string filePath);

    private:
        ImGuiTheme& m_Theme;
    };
}