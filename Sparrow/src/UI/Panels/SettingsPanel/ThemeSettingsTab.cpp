#include "ThemeSettingsTab.h"

#include "Crane/Utils/PlatformUtils.h"
#include "Crane/Core/Application.h"
#include "Crane/ImGui/Serialization/ThemeSerializer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>



namespace Crane {
    ThemeSettingsTab::ThemeSettingsTab(bool isRequired)
        : Panel(isRequired)
    {
        Application::Get().GetImGuiLayer()->SetThemeColors(m_Theme);
    }

    void ThemeSettingsTab::OnImGuiRender()
    {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        auto& name = m_Theme.Name;
        strcpy(buffer, name.c_str());
        ImGui::Text("Name");
        if (ImGui::InputText("##", buffer, sizeof(buffer)))
        {
            auto name = std::string(buffer, sizeof(buffer));
            name.erase(name.find('\0'));
            m_Theme.Name = name;
        }
        if (ImGui::ColorEdit4("Primary", glm::value_ptr(m_Theme.Primary)) ||
            ImGui::ColorEdit4("Primary Light", glm::value_ptr(m_Theme.PrimaryLight)) ||
            ImGui::ColorEdit4("Primary Dark", glm::value_ptr(m_Theme.PrimaryDark)) ||
            ImGui::ColorEdit4("Sceondary", glm::value_ptr(m_Theme.Secondary)) ||
            ImGui::ColorEdit4("White", glm::value_ptr(m_Theme.White)) ||
            ImGui::ColorEdit4("Dark", glm::value_ptr(m_Theme.Dark)) ||
            ImGui::ColorEdit4("Medium", glm::value_ptr(m_Theme.Medium)) ||
            ImGui::ColorEdit4("Light", glm::value_ptr(m_Theme.Light)) ||
            ImGui::ColorEdit4("WindowBg", glm::value_ptr(m_Theme.WindowBg)))
            Application::Get().GetImGuiLayer()->SetThemeColors(m_Theme);
        if (ImGui::Button("Save Theme"))
            SaveTheme();
        ImGui::SameLine();
        if (ImGui::Button("Open Theme"))
            OpenTheme();
    }

    void ThemeSettingsTab::SaveTheme()
    {
        std::string name = m_Theme.Name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        std::string filePath = "assets/settings/themes/" + name + ".theme";

        ThemeSerializer serializer(m_Theme);
        serializer.Serialize(filePath);
    }

    void ThemeSettingsTab::OpenTheme()
    {
        std::string filePath = FileDialogs::OpenFile();
        if (!filePath.empty())
        {
            ThemeSerializer serializer(m_Theme);
            serializer.Deserialize(filePath);
            Application::Get().GetImGuiLayer()->SetThemeColors(m_Theme);
        }
    }
}