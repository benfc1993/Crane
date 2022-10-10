#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>


namespace Crane {

    static const std::filesystem::path s_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel(bool isRequired)
        : m_CurrentDirectory(s_AssetPath), Panel(isRequired)
    {
    }


    void ContentBrowserPanel::OnImGuiRender()
    {
        std::string name = "ContentBrowserPanel" + std::to_string(m_Index);
        ImGui::Begin(name.c_str());
        ImGui::SliderFloat("Size", &m_ViewSize, 0.0f, 1.0f, "%.1f");

        ImGui::SetWindowFontScale(((m_ViewSize * 0.1f) + 0.1f) / 0.1f);
        if (m_ViewSize <= 0.2f)
        {
            DrawDirectoryTree(s_AssetPath);
        }
        else
        {
            DrawDirectoryThumbnails();
        }

        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();
    }

    void ContentBrowserPanel::DrawDirectoryTree(std::filesystem::path path)
    {
        for (auto& entry : std::filesystem::directory_iterator(path))
        {
            std::string filename = entry.path().filename().string();

            if (entry.is_directory())
            {
                if (ImGui::TreeNode(filename.c_str()))
                {
                    DrawDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
                continue;
            }
            if (ImGui::Selectable(filename.c_str()))
                CR_INFO("Selected File: {0}", filename.c_str());
        }
    }

    void ContentBrowserPanel::DrawDirectoryThumbnails()
    {
        ImGui::Text("%s", m_CurrentDirectory.string().c_str());
        if (m_CurrentDirectory != s_AssetPath)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            const auto& path = entry.path();
            std::string pathString = path.string();
            std::string filename = entry.path().filename().string();

            if (entry.is_directory())
            {
                if (ImGui::Button(filename.c_str()))
                {
                    m_CurrentDirectory /= path.filename();
                }
                continue;
            }
            if (ImGui::Button(filename.c_str()))
                CR_INFO("Selected File: {0}", filename);
        }
    }
}