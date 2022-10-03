#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Scene/Scene.h"

namespace Crane {
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string filePath);
        void SerializeRuntime(const std::string& filePath);

        bool Deserialize(const std::string filePath);
        bool DeserializeRuntime(const std::string& filePath);

    private:
        Ref<Scene> m_scene;
    };
}