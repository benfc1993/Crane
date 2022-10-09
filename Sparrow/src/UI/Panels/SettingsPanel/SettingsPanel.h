#pragma once

#include "Crane.h"

#include "UI/Panels/Panel.h"

#include "ThemeSettingsTab.h"

#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
    class SettingsPanel : public Panel
    {
    public:

        SettingsPanel(bool isRequired = false);

        virtual void OnImGuiRender() override;

        void OpenPanel() { m_Open = true; }
        void ClosePanel() { m_Open = false; }

    private:
        bool m_Open = false;
        Ref<ThemeSettingsTab> m_ThemeSettingsTab = nullptr;
    };
}