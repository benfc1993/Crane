#include "Crane/ImGui/ImGuiTheme.h"

namespace Crane {
    class SettingsPanel
    {
    public:

        SettingsPanel() {}

        void OnImGuiRender();

        void SetTheme(ImGuiTheme* theme) { m_Theme = theme; }

        void OpenPanel() { m_Open = true; }
        void ClosePanel() { m_Open = false; }

    private:
        void ThemeSettingsTab();
        bool m_Open = false;
        ImGuiTheme* m_Theme = nullptr;
    };
}