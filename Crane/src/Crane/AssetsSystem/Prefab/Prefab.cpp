#include "Prefab.h"

#include "Crane/Core/Application.h"
#include "Crane/AssetsSystem/Prefab/Serialisation/PrefabSerialiser.h"


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

}