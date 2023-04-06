#pragma once


#include "Crane/Scene/Entity.h"

#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace Crane {
	class PrefabSerialiser
	{
	public:
		static void SerialisePrefab(YAML::Emitter& out, const Ref<Scene>& scene, Entity& entity, bool prefabScene);
		static std::filesystem::path SerialisePrefab(const Ref<Scene>& scene, Entity& entity, std::filesystem::path filepath, bool prefabScene);
		static Entity DeserialisePrefab(UUID prefabAssetHandle, const Ref<Scene>& scene, bool resetPosition = false);
		static Entity DeserialisePrefab(std::filesystem::path filepath, const Ref<Scene>& scene, bool resetPosition = false, bool asScene = false);
	};
}