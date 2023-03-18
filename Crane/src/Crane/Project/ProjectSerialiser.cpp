#include "crpch.h"
#include "ProjectSerialiser.h"


#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Crane {
    ProjectSerializer::ProjectSerializer(Ref<Project> project): m_Project(project)
    {
    }

    bool ProjectSerializer::Serialize(const std::filesystem::path& filepath)
    {
        const auto& config = m_Project->GetConfig();

        YAML::Emitter out;
        out << YAML::BeginMap; // Root
        {
            out << YAML::Key << "Project" << YAML::Value;
            out << YAML::BeginMap; // Project
            {
                out << YAML::Key << "Name" << YAML::Value << config.Name;
                out << YAML::Key << "ProjectRootDir" << YAML::Value << config.ProjectRootDir.string();
                out << YAML::Key << "AssetDir" << YAML::Value << config.AssetDir.string();
                out << YAML::Key << "AssemblyPath" << YAML::Value << config.AssemblyPath.string();
                out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
            }
            out << YAML::EndMap; // Project
        }
        out << YAML::EndMap; // Root

        std::ofstream fout(filepath.string());
        fout << out.c_str();
        return true;
    }

    bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
    {
        auto& config = m_Project->GetConfig();

        std::ifstream stream(filepath.string());
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data;
        try
        {
            data = YAML::Load(strStream.str());
        }
        catch (YAML::ParserException e)
        {
            CR_CORE_ERROR("Failed to load project file '{0}' \n     {1}", filepath.string(), e.what());
            return false;
        }

        auto projectNode = data["Project"];

        if (!projectNode)
        {
            CR_CORE_INFO("No project");
            return false;
        }

        config.Name = projectNode["Name"].as<std::string>();
        config.ProjectRootDir = projectNode["ProjectRootDir"].as<std::string>();
        config.AssetDir = projectNode["AssetDir"].as<std::string>();
        config.AssemblyPath = projectNode["AssemblyPath"].as<std::string>();
        config.StartScene = projectNode["StartScene"].as<std::string>();

        return true;
    }
}


