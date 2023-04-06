#include "crpch.h"
#include "PrefabSerialiser.h"

#include "Crane/Core/Application.h"
#include "Crane/Scene/Serialisation/SceneSerialiser.h"

namespace Crane {

	void SerialiseChildren(const Ref<Scene>& scene, Entity entity, YAML::Emitter& out, bool prefabScene)
	{
		auto current = entity.GetComponent<HierarchyComponent>().First;
		while (current != 0)
		{
			auto child = scene->GetEntityByUUID(current);

			if (!child.HasComponent<PrefabComponent>())
			{
				child.AddComponent<PrefabComponent>();
			}

			auto& pc = child.GetComponent<PrefabComponent>();
			auto id = child.GetComponent<IdComponent>();
			pc.PrefabEntityId = id.Id;

			auto sceneSerialiser = SceneSerialiser(scene);

			auto hc = child.GetComponent<HierarchyComponent>();
			if (hc.First != 0)
			{
				SerialiseChildren(scene, child, out, prefabScene);
			}
			sceneSerialiser.SerialiseEntity(out, child, prefabScene);
			current = child.GetComponent<HierarchyComponent>().Next;
		}
	}

	std::filesystem::path PrefabSerialiser::SerialisePrefab(const Ref<Scene>& scene, Entity& entity, std::filesystem::path filepath, bool prefabScene)
	{

		YAML::Emitter out;
		auto& tc = entity.GetComponent<TransformComponent>();
		tc.Position = { 0.0f, 0.0f, 0.0f };
		SerialisePrefab(out, scene, entity, prefabScene);
		std::ofstream fout(filepath.c_str());
		fout << out.c_str();

		return filepath;
	}

	void PrefabSerialiser::SerialisePrefab(YAML::Emitter& out, const Ref<Scene>& scene, Entity& entity, bool prefabScene)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Prefab" << YAML::Value << YAML::BeginMap; // Scene
		std::string name = entity.GetName();
		out << YAML::Key << "Name" << YAML::Value << name;
		out << YAML::Key << "AssetHandle" << YAML::Value << entity.GetComponent<PrefabComponent>().AssetHandle;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap; // Entities

		auto sceneSerialiser = SceneSerialiser(scene);
		sceneSerialiser.SerialiseEntity(out, entity, prefabScene);

		SerialiseChildren(scene, entity, out, prefabScene);

		out << YAML::EndMap; // Entities
		out << YAML::EndMap;
	}



	Entity PrefabSerialiser::DeserialisePrefab(UUID prefabHandle, const Ref<Scene>& scene, bool resetPosition)
	{
		auto filePath = Application::Get().GetAssetRegistry()->GetAsset(prefabHandle).FilePath;
		return DeserialisePrefab(filePath, scene, resetPosition);
	}

	Entity PrefabSerialiser::DeserialisePrefab(std::filesystem::path filepath, const Ref<Scene>& scene, bool resetPosition, bool asScene)
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
			for (YAML::const_iterator it = entities.begin();it != entities.end();++it)
			{
				std::string key = it->first.as<std::string>();
				YAML::Node entity = entities[key];

				auto beforeId = UUID(entity["Entity"].as<uint64_t>());

				auto sceneSerialiser = SceneSerialiser(scene);
				Entity deserialisedEntity = sceneSerialiser.DeserialiseEntity(entity, !asScene);

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

				if (hc.Parent == 0)
				{
					parentEntity = entity;
					if (resetPosition)
					{
						auto& tc = entity.GetComponent<TransformComponent>();

						tc.Position = { 0.0f, 0.0f, 0.0f };
					}
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
