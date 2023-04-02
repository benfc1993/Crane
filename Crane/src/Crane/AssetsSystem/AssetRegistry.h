#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Core/UUID.h"

#include <string>
#include <unordered_map>
#include <fstream>


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

		inline std::string AssetTypeToString(AssetType type)
		{
			switch (type)
			{
			case AssetType::Prefab: return "Prefab";
			default: return "None";
			}
		}

		inline AssetType AssetTypeFromString(std::string str)
		{
			if (str == "Prefab") return AssetType::Prefab;

			CR_ASSERT(false, "Invalid asset type");
			return AssetType::None;
		}

	}


	struct Asset
	{
		Asset(std::string name, const std::filesystem::path filePath, AssetType type)
		{
			Name = name;
			Type = type;
			FilePath = filePath;
		}
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
		void SaveAssetRegistry(std::filesystem::path filePath);

		bool LoadAssetRegistry(std::filesystem::path filePath);

		Asset GetAsset(UUID handle)
		{
			CR_ASSERT(m_registry.find(handle) != m_registry.end());
			return m_registry.at(handle);
		}

		void RegisterAsset(UUID handle, Asset asset)
		{
			m_registry.emplace(handle, asset);
		}

		void Register(std::function<void(UUID, AssetType)> subscriber)
		{
			m_Subscribers.emplace_back(subscriber);
		}

		void OnAssetChanged(UUID handle)
		{
			if (m_registry.find(handle) == m_registry.end())
			{
				CR_CORE_ERROR("Asset not registered: {}", handle);
				return;
			}

			for (auto subscriber : m_Subscribers)
			{
				subscriber(handle, m_registry.at(handle).Type);
			}
		}
	private:
		std::unordered_map<UUID, Asset> m_registry;
		std::vector<std::function<void(UUID, AssetType)>> m_Subscribers;
	};


}