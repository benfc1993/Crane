#include "crpch.h"
#include "Prefab.h"

#include "Crane/Core/Application.h"
#include "Crane/AssetsSystem/Prefab/Serialisation/PrefabSerialiser.h"
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

		// Update the entity's Position component
		// registry.patch<Position>(entity, [](auto& pos) {
		// 	pos.x = 10.0f;
		// 	pos.y = 20.0f;
		// });

		// Serialize the updated entity to a YAML file
		YAML::Emitter out;
		PrefabSerialiser::SerialisePrefab(out, scene, entity);
		YAML::Node updated = YAML::Load(out.c_str());
		updated = updated["Prefab"]["Entities"];

		YAML::Emitter diff;

		diff << YAML::BeginSeq;

		if (updated.IsSequence())
		{
			for (std::size_t i = 0;i < updated.size();i++)
			{
				CR_CORE_INFO("UUID: {}", updated[i]["Entity"].as<uint64_t>());
				diff << YAML::BeginMap;
				diff << YAML::Key << "Entity" << YAML::Value << updated[i]["Entity"].as<uint64_t>();
				for (auto up : updated[i])
				{
					auto key = up.first.as<std::string>();
					if (updated[i][key].IsMap())
					{
						bool componentDiff = false;
						diff << YAML::Key << key;
						diff << YAML::BeginMap;


						for (auto obj : updated[i][key])
						{
							CR_CORE_INFO("TYPE: {}", obj.second.Type());
							auto objKey = obj.first.as<std::string>();
							switch (obj.second.Type())
							{
							case YAML::NodeType::value::Scalar:
							{
								auto u = obj.second.as<std::string>();
								auto o = updated[i][key][objKey].as<std::string>();
								if (u != o)
								{
									componentDiff = true;
									diff << YAML::Key << "From" << YAML::Value << o;
									diff << YAML::Key << "To" << YAML::Value << u;

									CR_CORE_INFO("Component: {} ~ Key: {} - Updated: {} || original: {}", key, objKey, u, o);
								}
							}
							break;
							case YAML::NodeType::value::Sequence:
							{
								if (obj.second.size() == 2)
								{
									auto u = obj.second.as<glm::vec2>();
									auto o = original[i][key][objKey].as<glm::vec2>();

									if (u != o)
									{
										componentDiff = true;
										diff << YAML::Key << "From" << YAML::Value << o;
										diff << YAML::Key << "To" << YAML::Value << u;

										CR_CORE_INFO("Component: {} ~ Key: {} - Updated: {} || original: {}", key, objKey, u, o);
									}
								}

								if (obj.second.size() == 3)
								{
									auto u = obj.second.as<glm::vec3>();
									auto o = original[i][key][objKey].as<glm::vec3>();

									if (u != o)
									{
										componentDiff = true;
										diff << YAML::Key << "From" << YAML::Value << o;
										diff << YAML::Key << "To" << YAML::Value << u;

										CR_CORE_INFO("Component: {} ~ Key: {} - Updated: {} || original: {}", key, objKey, u, o);
									}
								}

								if (obj.second.size() == 4)
								{
									auto u = obj.second.as<glm::vec4>();
									auto o = original[i][key][objKey].as<glm::vec4>();
									if (u != o)
									{
										componentDiff = true;
										diff << YAML::Key << "From" << YAML::Value << o;
										diff << YAML::Key << "To" << YAML::Value << u;

										CR_CORE_INFO("Component: {} ~ Key: {} - Updated: {} || original: {}", key, objKey, u, o);
									}
								}
							}
							break;
							default:
							{
								auto u = obj.second.as<std::string>();
								auto o = original[i][key][objKey].as<std::string>();

								if (u != o)
								{
									componentDiff = true;
									diff << YAML::Key << "From" << YAML::Value << o;
									diff << YAML::Key << "To" << YAML::Value << u;

									CR_CORE_INFO("Component: {} ~ Key: {} - Updated: {} || original: {}", key, objKey, u, o);
								}
							}
							break;
							}

						}
						diff << YAML::EndMap;
					}
					else
					{
						auto u = updated[i][key].as<std::string>();
						auto o = updated[i][key].as<std::string>();
					}
				}

				diff << YAML::EndMap;
			}
		}
		diff << YAML::EndSeq;

		const std::string p = ((std::filesystem::path)prefabAsset.FilePath).replace_extension(".diff");

		std::ofstream fout(p.c_str());
		fout << diff.c_str();
	}
}