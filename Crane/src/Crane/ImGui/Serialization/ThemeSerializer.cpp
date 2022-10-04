#include "crpch.h"

#include "ThemeSerializer.h"

#include "Crane/Serialization/YamlOperators.h"
#include "Crane/Serialization/YamlDecoders.h"

#include <glm/glm.hpp>

namespace Crane {
    ThemeSerializer::ThemeSerializer(ImGuiTheme& theme)
        : m_Theme(theme)
    {

    }

    void ThemeSerializer::Serialize(const std::string filePath, const std::string name)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;

        out << YAML::Key << "Theme" << YAML::Value << YAML::BeginMap; // Theme
        out << YAML::Key << "Name" << YAML::Value << name;
        out << YAML::Key << "FilePath" << YAML::Value << filePath;
        out << YAML::EndMap; // Theme

        out << YAML::Key << "Colors" << YAML::Value; // Colors
        out << YAML::BeginMap; // Colors

        out << YAML::Key << "Primary" << YAML::Value << m_Theme.Primary;
        out << YAML::Key << "PrimaryLight" << YAML::Value << m_Theme.PrimaryLight;
        out << YAML::Key << "PrimaryDark" << YAML::Value << m_Theme.PrimaryDark;
        out << YAML::Key << "Secondary" << YAML::Value << m_Theme.Secondary;
        out << YAML::Key << "White" << YAML::Value << m_Theme.White;
        out << YAML::Key << "Dark" << YAML::Value << m_Theme.Dark;
        out << YAML::Key << "Medium" << YAML::Value << m_Theme.Medium;
        out << YAML::Key << "Light" << YAML::Value << m_Theme.Light;
        out << YAML::Key << "WindowBg" << YAML::Value << m_Theme.WindowBg;

        out << YAML::EndMap; // Colors

        out << YAML::EndMap;

        std::ofstream fout(filePath.c_str());
        fout << out.c_str();
    }

    void DeSerializeColor(glm::vec4 color, float(&themeValue)[4])
    {
        for (int i = 0; i < 4; i++)
            themeValue[i] = color[i];
    }

    bool ThemeSerializer::Deserialize(const std::string filePath)
    {
        std::ifstream stream(filePath.c_str());
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Theme"])
        {
            CR_CORE_INFO("No theme");
            return false;
        }

        m_Theme.Name = data["Theme"]["Name"].as<std::string>();
        m_Theme.FilePath = data["Theme"]["FilePath"].as<std::string>();
        CR_CORE_TRACE("Deserializing theme {0}", m_Theme.Name);

        YAML::Node colors = data["Colors"];

        m_Theme.Primary = colors["Primary"].as<glm::vec4>();
        m_Theme.PrimaryLight = colors["PrimaryLight"].as<glm::vec4>();
        m_Theme.PrimaryDark = colors["PrimaryDark"].as<glm::vec4>();
        m_Theme.Secondary = colors["Secondary"].as<glm::vec4>();
        m_Theme.White = colors["White"].as<glm::vec4>();
        m_Theme.Dark = colors["Dark"].as<glm::vec4>();
        m_Theme.Medium = colors["Medium"].as<glm::vec4>();
        m_Theme.Light = colors["Light"].as<glm::vec4>();
        m_Theme.WindowBg = colors["WindowBg"].as<glm::vec4>();

        return true;
    }
}