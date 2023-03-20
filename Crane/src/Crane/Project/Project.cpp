#include "crpch.h"
#include "Project.h"
#include "ProjectSerialiser.h"
#include <sys/wait.h> 

namespace Crane {

    Project::Project(const std::string& projectName, const std::filesystem::path& filepath)
    {
        auto ext = filepath.extension();
        m_Config.Name = projectName.c_str();
        m_Config.ProjectRootDir = filepath.string();
        m_Config.AssetDir = "Assets";
        m_Config.AssemblyPath = fmt::format("bin/{}.dll", projectName);
        m_Config.StartScene = "default.scene";
    }


    Ref<Project> Project::New(const std::string& projectName, const std::filesystem::path& filepath)
    {
        std::filesystem::path rootDir = filepath / projectName;
        s_ActiveProject = CreateRef<Project>(projectName, rootDir);
        CreatCodeProject(projectName, filepath);
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

    int Project::CreatCodeProject(const std::string& projectName, const std::filesystem::path& filepath)
    {
        int status;
        auto pid = fork();

        if (pid == 0)
        {
            execl("/bin/bash", "bash", "-c", fmt::format("./Resources/InitialProject/setupProject.sh {} {} {}", filepath, projectName, (std::filesystem::current_path() / "Resources/InitialProject/initialFiles").c_str()).c_str(), nullptr);
            exit(EXIT_FAILURE);
        }
        else if (pid < 0)
            status = -1;
        else
            if (waitpid(pid, &status, 0) != pid)
                status = -1;
        return status;
    }
}



