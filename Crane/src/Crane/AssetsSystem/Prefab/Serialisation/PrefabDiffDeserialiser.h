#pragma once

#include "Crane/Scene/Scene.h"
#include "Crane/Core/Base.h"
#include "Crane/AssetsSystem/AssetRegistry.h"

#include <filesystem>

namespace Crane {
    class PrefabDiffDeserialiser
    {
    public:
        static void DeserialiseDiff(Ref<Scene> scene, Asset prefabAsset);
    };
}