#include "EditorSettings.h"


#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Crane {

	static EditorSettingsData* s_Data;
	const std::string s_EditorSettingsFilePath = "assets/settings/editor.settings";

	EditorSettings::EditorSettings()
	{
		s_Data = new EditorSettingsData();
	}

	EditorSettings::~EditorSettings()
	{
		delete s_Data;
	}

	EditorSettingsData* EditorSettings::Get()
	{
		return s_Data;
	}

	void EditorSettings::Serialise()
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Settings" << YAML::Value << YAML::BeginMap; // Scene
		out << YAML::Key << "ShowColliders" << YAML::Value << s_Data->ShowColiders;

		out << YAML::EndMap;
		out << YAML::EndMap;

		std::ofstream fout(s_EditorSettingsFilePath.c_str());
		fout << out.c_str();
	}

	bool EditorSettings::Deserialise()
	{
		std::ifstream stream(s_EditorSettingsFilePath.c_str());
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Settings"])
		{
			CR_INFO("No scene");
			return false;
		}
		YAML::Node settings = data["Settings"];

		s_Data->ShowColiders = settings["ShowColliders"].as<bool>();
		return true;
	}
}