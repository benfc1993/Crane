#include "crpch.h"

#include "SceneSerialiser.h"
#include "ComponentSerialiser.h"
#include "ComponentDeserialiser.h"

#include "Crane/Scene/Components.h"

#include <filesystem>

namespace Crane {
	SceneSerialiser::SceneSerialiser(const Ref<Scene>& scene)
		: m_scene(scene)
	{

	}

	template<typename... Component>
	static void SerialiseComponents(ComponentGroup<Component...>, YAML::Emitter& out, Entity entity)
	{
		([&]()
		{
			ComponentSerialiser::SerialiseComponent<Component>(out, entity);
		}(), ...);
	}

	template<typename... Component>
	static void DeserialiseComponents(ComponentGroup<Component...>, YAML::Node& entity, Entity& deserialisedEntity)
	{
		([&]()
		{
			ComponentDeserialiser::DeserialiseComponent<Component>(entity, deserialisedEntity);
		}(), ...);
	}

	void SceneSerialiser::SerialiseEntity(YAML::Emitter& out, Entity entity)
	{
		CR_CORE_ASSERT(entity.HasComponent<IdComponent>());

		out << YAML::Key << entity.GetUUID();
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

		ComponentSerialiser::SerialiseComponent<TagComponent>(out, entity);

		ComponentSerialiser::SerialiseComponent<HierarchyComponent>(out, entity);

		if (entity.HasComponent<PrefabComponent>())
		{
			ComponentSerialiser::SerialiseComponent<PrefabComponent>(out, entity);
		}

		SerialiseComponents(AllComponents{}, out, entity);

		out << YAML::EndMap; // Entity
	}

	void SceneSerialiser::Serialise(const std::string filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; // root
		out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap; // Scene
		out << YAML::Key << "Name" << YAML::Value << "Untitled";
		out << YAML::Key << "FilePath" << YAML::Value << filePath;
		out << YAML::EndMap; // Scene
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap; // Entities

		m_scene->m_Registry.each([&](auto entityId) {
			Entity entity = { entityId , m_scene.get() };

			if (!entity)
				return;

			SerialiseEntity(out, entity);
		});

		out << YAML::EndMap; // Entities
		out << YAML::EndMap; // root

		std::ofstream fout(filePath.c_str());
		fout << out.c_str();
	}
	void SceneSerialiser::SerialiseRuntime(const std::string& filePath)
	{
		CR_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerialiser::Deserialise(const std::string filePath)
	{
		std::ifstream stream(filePath.c_str());
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		// if (!data["Scene"])
		// {
		// 	CR_CORE_INFO("No scene");
		// 	return false;
		// }

		// std::string sceneName = data["Scene"]["Name"].as<std::string>();
		// m_scene->SetFilePath(data["Scene"]["FilePath"].as<std::string>());
		// CR_CORE_TRACE("Deserialising scene {0}", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (YAML::const_iterator it = entities.begin();it != entities.end();++it)
			{
				std::string key = it->first.as<std::string>();       // <- key
				DeserialiseEntity(entities[key]);
			}
		}

		return true;
	}

	Entity SceneSerialiser::DeserialiseEntity(YAML::Node entity, bool prefab)
	{
		uint64_t uuid;
		if (prefab)
		{
			uuid = UUID();
		}
		else
		{
			uuid = entity["Entity"].as<uint64_t>();
		}

		std::string name;
		auto tagComponent = entity["TagComponent"];
		name = tagComponent["Tag"].as<std::string>();

		Entity deserialisedEntity = m_scene->CreateEntityWithUUID(uuid, name);

		DeserialiseComponents(AllComponents{}, entity, deserialisedEntity);

		ComponentDeserialiser::DeserialiseComponent<HierarchyComponent>(entity, deserialisedEntity);

		ComponentDeserialiser::DeserialiseComponent<PrefabComponent>(entity, deserialisedEntity);

		return deserialisedEntity;
	}

	bool SceneSerialiser::DeserialiseRuntime(const std::string& filePath)
	{
		CR_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}