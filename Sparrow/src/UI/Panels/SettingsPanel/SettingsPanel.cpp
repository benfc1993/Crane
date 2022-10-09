#include "SettingsPanel.h"

#include <Crane.h>

#include "imgui/imgui.h"

namespace Crane {

    SettingsPanel::SettingsPanel(bool isRequired)
        : Panel(isRequired)
    {
        m_ThemeSettingsTab = CreateRef<ThemeSettingsTab>();
    }

    void SettingsPanel::OnImGuiRender()
    {
        static bool p_Open = true;
        if (!m_Open) return;
        std::string name = "Settings###" + std::to_string(m_Index);
        if (!ImGui::Begin(name.c_str(), &p_Open, ImGuiWindowFlags_NoCollapse))
        {
            m_Open = p_Open;
            p_Open = true;
            ImGui::End();
            return;
        }


        ImGui::BeginTabBar("Settings");

        ImGui::BeginTabItem("Theme");

        m_ThemeSettingsTab->OnImGuiRender();

        ImGui::EndTabItem();

        ImGui::EndTabBar();

        ImGui::End();

    }
}