#include "crpch.h"
#include "PrefabSerialiser.h"

#include "Crane/Scene/Serialisation/SceneSerialiser.h"

#include <yaml-cpp/yaml.h>


namespace Crane {

	void SerialiseChildren(const Ref<Scene>& scene, Entity entity, YAML::Emitter& out)
	{
		auto current = entity.GetComponent<HierarchyComponent>().First;
		while (current != 0)
		{
			auto child = scene->GetEntityByUUID(current);

			auto sceneSerialiser = SceneSerialiser(scene);

			auto hc = child.GetComponent<HierarchyComponent>();
			if (hc.First != 0)
			{
				SerialiseChildren(scene, child, out);
			}
			sceneSerialiser.SerialiseEntity(out, child);
			current = child.GetComponent<HierarchyComponent>().Next;
		}
	}

	std::filesystem::path PrefabSerialiser::SerialisePrefab(const Ref<Scene>& scene, Entity& entity, std::filesystem::path filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Prefab" << YAML::Value << YAML::BeginMap; // Scene
		std::string name = entity.GetName();
		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "AssetHandle" << YAML::Value << entity.GetComponent<PrefabComponent>().AssetHandle;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // Entities

		auto sceneSerialiser = SceneSerialiser(scene);
		sceneSerialiser.SerialiseEntity(out, entity);

		SerialiseChildren(scene, entity, out);


		out << YAML::EndSeq; // Entities
		out << YAML::EndMap;


		std::ofstream fout(filepath.c_str());
		fout << out.c_str();

		return filepath;
	}


	Entity PrefabSerialiser::DeserialisePrefab(std::filesystem::path filepath, const Ref<Scene>& scene, bool resetPosition)
	{
		std::unordered_map<UUID, UUID> entityIdConversions;
		std::vector<Entity> DeserialisedEntities;

		std::ifstream stream(filepath.c_str());
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Prefab"])
		{
			CR_CORE_INFO("No prefab");
			return {};
		}

		Entity parentEntity;

		auto entities = data["Prefab"]["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				auto beforeId = UUID(entity["Entity"].as<uint64_t>());

				auto sceneSerialiser = SceneSerialiser(scene);
				Entity deserialisedEntity = sceneSerialiser.DeserialiseEntity(entity, true);

				auto afterId = deserialisedEntity.GetUUID();
				entityIdConversions.emplace(beforeId, afterId);
				DeserialisedEntities.push_back(deserialisedEntity);
			}

			for (auto entity : DeserialisedEntities)
			{
				auto& hc = entity.GetComponent<HierarchyComponent>();
				if (hc.First != 0)
				{
					hc.First = entityIdConversions.at(hc.First);
				}

				if (hc.Parent == 0 && resetPosition)
				{
					parentEntity = entity;
					auto& tc = entity.GetComponent<TransformComponent>();

					tc.Position = { 0.0f, 0.0f, 0.0f };
				}
				else
				{
					hc.Parent = entityIdConversions.at(hc.Parent);
				}

				if (hc.Prev != 0)
				{
					hc.Prev = entityIdConversions.at(hc.Prev);
				}

				if (hc.Next != 0)
				{
					hc.Next = entityIdConversions.at(hc.Next);
				}

			}
		}

		return parentEntity;
	}
}
