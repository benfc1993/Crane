#pragma once

#include "Crane/Scene/Entity.h"

#include <filesystem>

namespace Crane {
	class PrefabSerialiser
	{
	public:
		static std::filesystem::path SerialisePrefab(const Ref<Scene>& scene, Entity& entity, std::filesystem::path filepath);
		static Entity DeserialisePrefab(UUID prefabAssetHandle, const Ref<Scene>& scene, bool resetPosition = false);
		static Entity DeserialisePrefab(std::filesystem::path filepath, const Ref<Scene>& scene, bool resetPosition = false);
	};
}