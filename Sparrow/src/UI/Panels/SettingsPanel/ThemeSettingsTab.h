#pragma once

#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
    class ThemeSettingsTab
    {
    public:
        ThemeSettingsTab();

        void OnImGuiRender();

        void SaveTheme();
        void OpenTheme();

    private:
        ImGuiTheme m_Theme;
    };

}