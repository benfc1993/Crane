#include "crpch.h"
#include "Prefab.h"

#include "Crane/Core/Application.h"
#include "Crane/AssetsSystem/Prefab/Serialisation/PrefabSerialiser.h"
#include "Crane/AssetsSystem/Prefab/Serialisation/ComponentDiffSerialiser.h"
#include "Crane/Scene/Serialisation/SceneSerialiser.h"
#include "Crane/Serialisation/YamlDecoders.h"
#include "Crane/Serialisation/YamlOperators.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
	void Prefab::CreatePrefabFile(Ref<Scene> scene, UUID uuid, std::filesystem::path& dirPath)
	{

		Entity entity = scene->GetEntityByUUID(uuid);

		bool fileExists = std::filesystem::exists(dirPath / fmt::format("{}.prefab", entity.GetName()));

		bool hasPrefabComponent = entity.HasComponent<PrefabComponent>();

		auto fileName = fmt::format("{}.prefab", entity.GetName());

		if (!hasPrefabComponent)
		{
			auto handle = UUID();

			auto& pc = entity.AddComponent<PrefabComponent>();
			pc.AssetHandle = handle;
			if (fileExists)
			{
				int i = 0;
				const auto entityName = entity.GetName();
				fileName = fmt::format("{}({}).prefab", entityName, i);
				while (std::filesystem::exists(dirPath / fileName))
				{
					i++;
					fileName = fmt::format("{}({}).prefab", entityName, i);
				}
			}
		}

		auto prefabPath = PrefabSerialiser::SerialisePrefab(scene, entity, dirPath / fileName);

		auto assetHandle = entity.GetComponent<PrefabComponent>().AssetHandle;

		Application::Get().GetAssetRegistry()->RegisterAsset(assetHandle, Asset(entity.GetName(), prefabPath));

		auto asset = Application::Get().GetAssetRegistry()->GetAsset(assetHandle);
	}

	void Prefab::GetDiff(Ref<Scene> scene, UUID prefabHandle, Entity entity)
	{
		auto prefabAsset = Application::Get().GetAssetRegistry()->GetAsset(prefabHandle);
		std::ifstream stream(prefabAsset.FilePath.c_str());
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node original = YAML::Load(strStream.str());
		original = original["Prefab"]["Entities"];

		YAML::Emitter out;
		PrefabSerialiser::SerialisePrefab(out, scene, entity);
		YAML::Node updated = YAML::Load(out.c_str());
		updated = updated["Prefab"]["Entities"];

		YAML::Node root;

		if (updated.IsMap())
		{
			for (YAML::const_iterator it = updated.begin();it != updated.end();++it)
			{
				std::string entityId = it->first.as<std::string>();

				bool entityDiff = false;

				YAML::Node entity;

				if (!original[entityId])
				{
					SceneSerialiser serialiser = SceneSerialiser(scene);

					root["Added"].push_back(it->second);
					continue;
				}

				for (auto entityComponent : updated[entityId])
				{
					auto componentKey = entityComponent.first.as<std::string>();
					if (updated[entityId][componentKey].IsMap())
					{
						bool componentDiff = false;

						if (!original[entityId][componentKey])
						{
							entityDiff = true;
							YAML::Node addedComponent;
							addedComponent[componentKey] = updated[entityId][componentKey];
							entity["Added"].push_back(addedComponent);
							continue;
						}

						auto updatedComponent = updated[entityId][componentKey];
						auto originalComponent = original[entityId][componentKey];

						auto component = ComponentDiffDeserialiser::DeserialiseComponentDiff(originalComponent, updatedComponent, componentKey, componentDiff);
						if (componentDiff)
						{
							entityDiff = true;
							entity[componentKey] = component;
						}
					}
				}

				for (auto up : original[entityId])
				{
					auto componentKey = up.first.as<std::string>();

					if (!updated[entityId][componentKey])
					{
						entityDiff = true;
						entity["Removed"].push_back(componentKey);
					}
				}

				if (entityDiff)
					root[entityId] = entity;
			}

			for (YAML::const_iterator it = original.begin();it != original.end();++it)
			{
				std::string entityId = it->first.as<std::string>();
				if (!updated[entityId])
					root["Removed"].push_back(entityId);
			}
		}

		const std::string p = ((std::filesystem::path)prefabAsset.FilePath).replace_extension(".diff");

		std::ofstream fout(p.c_str());
		fout << root;
	}
}
