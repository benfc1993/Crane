#include "crpch.h"

#include "SceneSerializer.h"
#include "ComponentSerializer.h"
#include "ComponentDeserializer.h"

#include "Crane/Scene/Components.h"

#include <yaml-cpp/yaml.h>
#include <filesystem>

namespace Crane {
    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
        : m_scene(scene)
    {

    }

    template<typename... Component>
    static void SerializeComponents(ComponentGroup<Component...>, YAML::Emitter& out, Entity entity)
    {
        ([&]()
        {
            ComponentSerializer::SerializeComponent<Component>(out, entity);
        }(), ...);
    }

    template<typename... Component>
    static void DeserializeComponents(ComponentGroup<Component...>, YAML::Node& entity, Entity& deserialisedEntity)
    {
        ([&]()
        {
            ComponentDeserializer::DeserializeComponent<Component>(entity, deserialisedEntity);
        }(), ...);
    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        CR_CORE_ASSERT(entity.HasComponent<IdComponent>());

        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

        ComponentSerializer::SerializeComponent<TagComponent>(out, entity);

        SerializeComponents(AllComponents{}, out, entity);

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::Serialize(const std::string filePath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene" << YAML::Value << YAML::BeginMap; // Scene
        out << YAML::Key << "Name" << YAML::Value << "Untitled";
        out << YAML::Key << "FilePath" << YAML::Value << filePath;
        out << YAML::EndMap; // Scene
        out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq; // Entities

        m_scene->m_Registry.each([&](auto entityId) {
            Entity entity = { entityId , m_scene.get() };

            if (!entity)
                return;

            SerializeEntity(out, entity);
        });

        out << YAML::EndSeq; // Entities
        out << YAML::EndMap;

        std::ofstream fout(filePath.c_str());
        fout << out.c_str();
    }
    void SceneSerializer::SerializeRuntime(const std::string& filePath)
    {
        CR_CORE_ASSERT(false, "Not implemented");
    }

    bool SceneSerializer::Deserialize(const std::string filePath)
    {
        std::ifstream stream(filePath.c_str());
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
        {
            CR_CORE_INFO("No scene");
            return false;
        }

        std::string sceneName = data["Scene"]["Name"].as<std::string>();
        m_scene->SetFilePath(data["Scene"]["FilePath"].as<std::string>());
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

                Entity deserializedEntity = m_scene->CreateEntityWithUUID(uuid, name);

                DeserializeComponents(AllComponents{}, entity, deserializedEntity);

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