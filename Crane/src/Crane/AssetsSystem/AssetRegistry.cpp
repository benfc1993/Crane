#include "crpch.h"

#include "AssetRegistry.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
    void AssetRegistry::SaveAssetRegistry(std::filesystem::path filePath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq; // Assets

        for (auto asset : m_registry)
        {
            out << YAML::BeginMap; // Asset

            out << "Handle" << asset.first;
            out << "Name" << asset.second.Name;
            out << "FilePath" << asset.second.FilePath;
            out << "Type" << Utils::AssetTypeToString(asset.second.Type);

            out << YAML::EndMap; // Asset
        }

        out << YAML::EndSeq; // Assets
        out << YAML::EndMap;

        std::ofstream fout(filePath.c_str());
        fout << out.c_str();
    }

    bool AssetRegistry::LoadAssetRegistry(std::filesystem::path filePath)
    {
        std::ifstream stream(filePath.c_str());
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Assets"])
        {
            CR_CORE_INFO("No registry");
            return false;
        }

        auto assets = data["Assets"];
        if (assets && assets.size() > 0)
        {
            for (auto asset : assets)
            {
                auto assetHandle = asset["Handle"].as<uint64_t>();
                Asset newAsset = Asset(assetHandle, asset["Name"].as<std::string>(), asset["FilePath"].as<std::string>(), Utils::AssetTypeFromString(asset["Type"].as<std::string>()));
                m_registry.emplace(assetHandle, newAsset);
            }
        }

        return true;
    }

}