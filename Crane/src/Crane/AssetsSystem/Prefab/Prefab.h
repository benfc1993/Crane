#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Scene/Entity.h"
#include "Crane/Scene/Scene.h"

#include <filesystem>

namespace Crane {
	class Prefab
	{
	public:
		static void CreatePrefabFile(Ref<Scene> scene, UUID uuid, std::filesystem::path& dirPath);
	};
}