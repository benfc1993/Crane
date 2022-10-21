#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>



namespace Crane {

    static const std::filesystem::path s_AssetPath = "assets";

    static float padding = 16.0f;
    static float thumbnailSize = 50.0f;

    ContentBrowserPanel::ContentBrowserPanel(void* editor, bool isRequired)
        : m_Editor(editor), m_CurrentDirectory(s_AssetPath), Panel(isRequired)
    {
        m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
        m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
    }


    void ContentBrowserPanel::OnImGuiRender()
    {
        std::string name = "ContentBrowserPanel" + std::to_string(m_Index);
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
        ImGui::SliderFloat("Size", &thumbnailSize, 30.0f, 128.0f, "%1.f");

        ImVec2 available = ImGui::GetContentRegionAvail();

        if (ImGui::BeginTable("Browser", 2, 0, available, ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable))
        {
            ImGui::TableSetupColumn("treeC", ImGuiTableColumnFlags_WidthFixed, available.x * 0.25f);
            ImGui::TableSetupColumn("thumbsC", ImGuiTableColumnFlags_WidthFixed, available.x * 0.75f);

            ImGui::TableNextColumn();

            if (ImGui::BeginTable("tree", 1, ImGuiTableFlags_ScrollY))
            {
                ImGui::TableNextColumn();
                DrawDirectoryTree(s_AssetPath);
                ImGui::EndTable();
            }

            ImGui::TableNextColumn();
            DrawDirectoryThumbnails();
            ImGui::EndTable();
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

                bool isInPath = m_CurrentDirectory.string().find(entry.path().filename()) != std::string::npos;
                ImGui::SetNextItemOpen(isInPath);
                bool selected = m_CurrentDirectory.filename() == entry.path().filename();
                bool isOpened = ImGui::TreeNodeEx(filename.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth | (selected ? ImGuiTreeNodeFlags_Selected : 0));

                if (ImGui::IsItemClicked())
                {
                    m_CurrentDirectory = entry.path();
                }

                if (isOpened)
                {
                    DrawDirectoryTree(entry.path());
                    ImGui::TreePop();
                }
            }
        }

    }

    void ContentBrowserPanel::DrawDirectoryThumbnails()
    {

        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;
        if (m_CurrentDirectory != s_AssetPath)
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        ImGui::SameLine();

        ImGui::Text("%s", m_CurrentDirectory.string().c_str());

        if (ImGui::BeginTable("thumbnails", columnCount, ImGuiTableFlags_ScrollY))
        {
            ImGui::TableNextColumn();

            for (auto& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
            {
                const auto& path = entry.path();
                auto relativePath = std::filesystem::relative(path, s_AssetPath);
                std::string pathString = path.string();
                std::string filename = entry.path().filename().string();

                ImGui::PushID(filename.c_str());
                Ref<Texture2D> icon = entry.is_directory() ? m_DirectoryIcon : m_FileIcon;

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImVec2 uv0{ 0, 1 }, uv1{ 1, 0 };
                ImGui::ImageButton("##", (ImTextureID)icon->GetRendererId(), { thumbnailSize, thumbnailSize }, uv0, uv1);
                ImGui::PopStyleColor();

                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    if (entry.is_directory())
                    {
                        m_CurrentDirectory /= path.filename();
                    }
                    else
                    {
                        ((EditorLayer*)m_Editor)->LoadScene(path);
                    }
                }

                if (ImGui::BeginDragDropSource())
                {
                    std::string filePath = relativePath.string();
                    ImGui::SetDragDropPayload("CONTENT_BROWSER_FILE", (void*)relativePath.c_str(), relativePath.string().length() + 1);
                    ImGui::Text("%s", filename.c_str());
                    ImGui::EndDragDropSource();
                }

                ImGui::TextWrapped("%s", filename.c_str());

                ImGui::TableNextColumn();

                ImGui::PopID();
            }
            ImGui::EndTable();
        }
    }
}