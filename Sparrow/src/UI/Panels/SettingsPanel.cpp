#include "SettingsPanel.h"

#include <Crane.h>

#include "imgui/imgui.h"


namespace Crane {

    void SettingsPanel::OnImGuiRender()
    {
        static bool p_Open = true;
        if (!m_Open) return;

        if (!ImGui::Begin("Settings", &p_Open, ImGuiWindowFlags_NoCollapse))
        {
            m_Open = p_Open;
            p_Open = true;
            ImGui::End();
            return;
        }


        ImGui::BeginTabBar("Settings");

        ImGui::BeginTabItem("Theme");

        ThemeSettingsTab();

        ImGui::EndTabItem();

        ImGui::EndTabBar();

        ImGui::End();

    }

    void SettingsPanel::ThemeSettingsTab()
    {
        if (ImGui::ColorEdit4("Primary", (float*)&m_Theme->Primary) ||
            ImGui::ColorEdit4("Primary Light", (float*)&m_Theme->PrimaryLight) ||
            ImGui::ColorEdit4("Primary Dark", (float*)&m_Theme->PrimaryDark) ||
            ImGui::ColorEdit4("Sceondary", (float*)&m_Theme->Secondary) ||
            ImGui::ColorEdit4("White", (float*)&m_Theme->White) ||
            ImGui::ColorEdit4("Dark", (float*)&m_Theme->Dark) ||
            ImGui::ColorEdit4("Medium", (float*)&m_Theme->Medium) ||
            ImGui::ColorEdit4("Light", (float*)&m_Theme->Light) ||
            ImGui::ColorEdit4("WindowBg", (float*)&m_Theme->WindowBg))
            Application::Get().GetImGuiLayer()->SetDarkThemeColors(*m_Theme);
    }
}