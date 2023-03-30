#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/UUID.h"

#include <string>
#include <unordered_map>


namespace Crane {
	enum class AssetType
	{
		None = 0,
		Prefab = 1,
	};

	namespace Utils {

		inline AssetType AssetTypeFromExtension(std::string extension)
		{
			if (extension == ".prefab") return AssetType::Prefab;

			CR_ASSERT(false, "Invalid asset extension");
			return AssetType::None;
		}

	}


	struct Asset
	{
		Asset(std::string name, const std::filesystem::path filePath)
		{
			Name = name;
			Type = Utils::AssetTypeFromExtension(filePath.extension());
			FilePath = filePath;
		}
		std::string Name;
		AssetType Type;
		std::string FilePath;
	};

	class AssetRegistry
	{
	public:
		Asset GetAsset(UUID handle)
		{
			CR_ASSERT(m_registry.find(handle) != m_registry.end());
			return m_registry.at(handle);
		}

		void RegisterAsset(UUID handle, Asset asset)
		{
			m_registry.emplace(handle, asset);
		}
	private:
		std::unordered_map<UUID, Asset> m_registry;
	};


}