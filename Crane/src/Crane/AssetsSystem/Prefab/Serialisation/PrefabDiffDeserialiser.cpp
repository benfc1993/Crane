#include "crpch.h"
#include "PrefabDiffDeserialiser.h"

#include "Crane/Scene/Components.h"

#include "Crane/Serialisation/YamlDecoders.h"
#include <yaml-cpp/yaml.h>

namespace Crane {
#define PATCH_VALUE(ValueProp, ValueName, valueType) if (diff[ValueName]) \
    { \
        auto oldValue = diff[ValueName]["From"].as<valueType>();\
        if (transform.ValueProp == oldValue) \
        { \
            auto newValue = diff[ValueName]["To"].as<valueType>(); \
            transform.ValueProp = newValue; \
        } \
    } \

    void PrefabDiffDeserialiser::DeserialiseDiff(Ref<Scene> scene, Asset prefabAsset)
    {
        std::filesystem::path filepath = prefabAsset.FilePath;
        std::ifstream stream(filepath.replace_extension(".diff").c_str());
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        auto view = scene->Reg().view<PrefabComponent>();
        for (auto entity : view)
        {
            auto pc = scene->Reg().get<PrefabComponent>(entity);
            if (pc.AssetHandle != prefabAsset.Handle)
                continue;

            auto prefabEntityId = std::to_string(pc.PrefabEntityId);

            if (!data[prefabEntityId]) continue;

            auto entityDiff = data[prefabEntityId];

            auto diff = entityDiff["TransformComponent"];

            if (diff)
            {
                scene->Reg().patch<TransformComponent>(entity, [&](TransformComponent& transform) {
                    PATCH_VALUE(WorldPosition, "WorldPosition", glm::vec3);
                    PATCH_VALUE(Position, "Position", glm::vec3);
                    PATCH_VALUE(Rotation, "Rotation", glm::vec3);
                    PATCH_VALUE(WorldScale, "WorldScale", glm::vec3);
                    PATCH_VALUE(Scale, "Scale", glm::vec3);
                });
            }
        }
    }

}