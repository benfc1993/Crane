#include "crpch.h"
#include "Project.h"
#include "ProjectSerialiser.h"

namespace Crane {

    Project::Project(const std::string& projectName, const std::filesystem::path& filepath)
    {
        auto ext = filepath.extension();
        CR_CORE_INFO("Project ext: {}", ext == "");
        m_Config.Name = projectName.c_str();
        m_Config.ProjectRootDir = ext == "" ? filepath.string() : filepath.parent_path().string();
        m_Config.AssetDir = "Assets";
        m_Config.AssemblyPath = fmt::format("Bin/{}.dll", projectName);
        m_Config.StartScene = "Physics.scene";
    }


    Ref<Project> Project::New(const std::string& projectName, const std::filesystem::path& filepath)
    {
        std::filesystem::path rootDir = filepath.parent_path();
        s_ActiveProject = CreateRef<Project>(projectName, rootDir);
        return s_ActiveProject;
    }

    Ref<Project> Project::Load(const std::filesystem::path& filepath)
    {

        if (filepath.extension().string() != ".crproj")
            return nullptr;

        Ref<Project> project = CreateRef<Project>("", "");
        ProjectSerializer serialiser = ProjectSerializer(project);
        if (serialiser.Deserialize(filepath))
        {
            s_ActiveProject = project;
            return s_ActiveProject;
        }
        return nullptr;
    }

    bool Project::Save(const std::filesystem::path& filepath)
    {
        ProjectSerializer serialiser = ProjectSerializer(s_ActiveProject);
        return serialiser.Serialize(filepath);
    }

}



