#pragma once

#include <string>
#include <filesystem>

#include "Crane/Core/Base.h"

namespace Crane {

    struct ProjectConfig
    {
        std::string Name;
        std::filesystem::path ProjectRootDir;
        std::filesystem::path AssetDir;
        std::filesystem::path AssemblyPath;
        std::filesystem::path StartScene;
    };


    class Project
    {
    public:
        Project(const std::string& projectName, const std::filesystem::path& filepath);

        std::filesystem::path GetRootPath()
        {
            return m_Config.ProjectRootDir;
        }

        std::filesystem::path GetScenePath()
        {
            return m_Config.ProjectRootDir / m_Config.AssetDir / "Scenes" / m_Config.StartScene;
        }

        std::filesystem::path GetAssetsPath()
        {
            return m_Config.ProjectRootDir / m_Config.AssetDir;
        }

        std::filesystem::path GetAssemblyPath()
        {
            return m_Config.ProjectRootDir / m_Config.AssemblyPath;
        }

        static Ref<Project> New(const std::string& projectName, const std::filesystem::path& filepath);
        static Ref<Project> Load(const std::filesystem::path& filepath);
        static bool Save(const std::filesystem::path& filepath);

        static int CreatCodeProject(const std::string& projectName, const std::filesystem::path& filepath);

        ProjectConfig& GetConfig() { return m_Config; }
        static Ref<Project> GetActive() { return s_ActiveProject; }
    private:
        ProjectConfig m_Config;
        inline static Ref<Project> s_ActiveProject;
    };
}