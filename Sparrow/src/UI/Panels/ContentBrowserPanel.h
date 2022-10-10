#pragma once

#include "Panel.h"

#include <filesystem>

namespace Crane {
    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel(bool isRequired = false);
        virtual void OnImGuiRender() override;
    private:
        void DrawDirectoryTree(std::filesystem::path path);
        void DrawDirectoryThumbnails();
    private:
        std::filesystem::path m_CurrentDirectory;
        float m_ViewSize = 0.0f;
    };
}