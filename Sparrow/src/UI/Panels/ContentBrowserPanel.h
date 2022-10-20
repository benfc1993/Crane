#pragma once

#include "Panel.h"

#include "Crane/Renderer/Shader/Texture.h"

#include "EditorLayer.h"

#include <filesystem>

namespace Crane {
    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel(void* editorLayer, bool isRequired = false);
        virtual void OnImGuiRender() override;
    private:
        void DrawDirectoryTree(std::filesystem::path path);
        void DrawDirectoryThumbnails();
    private:
        std::filesystem::path m_CurrentDirectory;
        float m_ViewSize = 0.0f;

        Ref<Texture2D> m_DirectoryIcon;
        Ref<Texture2D> m_FileIcon;

        void* m_Editor;
    };
}