#pragma once

#include "UI/Panels/Panel.h"

#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
    class ThemeSettingsTab : public Panel
    {
    public:
        ThemeSettingsTab(bool isRequired = false);

        virtual void OnImGuiRender() override;

        void SaveTheme();
        void OpenTheme();

    private:
        ImGuiTheme m_Theme;
    };

}