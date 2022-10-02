#include "crpch.h"

#include "SceneSerializer.h"
#include "ComponentSerializer.h"
#include "ComponentDeserializer.h"

#include "Crane/Scene/Components.h"

#include <yaml-cpp/yaml.h>

namespace Crane {
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_scene(scene)
    {

    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << "123523563245";


        ComponentSerializer::SerializeComponent<TagComponent>(out, entity);

        ComponentSerializer::SerializeComponent<TransformComponent>(out, entity);

        ComponentSerializer::SerializeComponent<CameraComponent>(out, entity);

        ComponentSerializer::SerializeComponent<SpriteRendererComponent>(out, entity);

        ComponentSerializer::SerializeComponent<ParticleSystemComponent>(out, entity);

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::Serialize(const std::string& filePath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << "Untitled";
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

        m_scene->m_Registry.each([&](auto entityId) {
            Entity entity = { entityId , m_scene.get() };

            if (!entity)
                return;

            SerializeEntity(out, entity);
        });

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filePath);
        fout << out.c_str();
    }
    void SceneSerializer::SerializeRuntime(const std::string& filePath)
    {
        CR_CORE_ASSERT(false, "Not implemented");
    }

    bool SceneSerializer::Deserialize(const std::string& filePath)
    {
        std::ifstream stream(filePath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        CR_CORE_TRACE("Deserializing scene {0}", sceneName);

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                std::string name;
                auto tagComponent = entity["TagComponent"];
                name = tagComponent["Tag"].as<std::string>();

                Entity deserializedEntity = m_scene->CreateEntity(name);

                ComponentDeserializer::DeserializeComponent<TransformComponent>(entity, deserializedEntity);

                ComponentDeserializer::DeserializeComponent<CameraComponent>(entity, deserializedEntity);

                ComponentDeserializer::DeserializeComponent<SpriteRendererComponent>(entity, deserializedEntity);

                ComponentDeserializer::DeserializeComponent<ParticleSystemComponent>(entity, deserializedEntity);

                CR_CORE_TRACE("Deserialized Entity Id = {0}, Name = {1}", uuid, name);
            }
        }

        return true;
    }
    bool SceneSerializer::DeserializeRuntime(const std::string& filePath)
    {
        CR_CORE_ASSERT(false, "Not implemented");
        return false;
    }
}