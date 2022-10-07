#include "crpch.h"

#include "ComponentDeserializer.h"

#include "Crane/Scene/Components.h"

#include "Crane/Serialization/YamlDecoders.h"

namespace Crane {

    template <>
    void ComponentDeserializer::DeserializeComponent<TransformComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["TransformComponent"];
        if (!node) return;

        auto& transformComponent = entity.AddOrReplaceComponent<TransformComponent>();

        transformComponent.Position = node["Position"].as<glm::vec3>();
        transformComponent.Rotation = node["Rotation"].as<glm::vec3>();
        transformComponent.Scale = node["Scale"].as<glm::vec3>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<CameraComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["CameraComponent"];
        YAML::Node cameraNode = node["Camera"];
        if (!node) return;

        auto& cameraComponent = entity.AddComponent<CameraComponent>();
        cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraNode["ProjectionType"].as<int>());
        cameraComponent.Camera.SetPerspectiveVerticalFov(cameraNode["PerspectiveVerticalFov"].as<float>());
        cameraComponent.Camera.SetPerspectiveNearClip(cameraNode["PerspectiveNearClip"].as<float>());
        cameraComponent.Camera.SetPerspectiveFarClip(cameraNode["PerspectiveFarClip"].as<float>());
        cameraComponent.Camera.SetOrthographicSize(cameraNode["OrthographicSize"].as<float>());
        cameraComponent.Camera.SetOrthographicNearClip(cameraNode["OrthographicNearClip"].as<float>());
        cameraComponent.Camera.SetOrthographicFarClip(cameraNode["OrthographicFarClip"].as<float>());
        cameraComponent.Camera.SetOrthographicFarClip(cameraNode["OrthographicFarClip"].as<float>());

        cameraComponent.FixedAspectRatio = (node["FixedAspectRatio"].as<bool>());
        cameraComponent.Primary = node["Primary"].as<bool>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<SpriteRendererComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["SpriteRendererComponent"];
        if (!node) return;

        auto& spriteRendererComponent = entity.AddComponent<SpriteRendererComponent>();
        spriteRendererComponent.Color = node["Color"].as<glm::vec4>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<ParticleSystemComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["ParticleSystemComponent"];
        if (!node) return;

        float particleCount = node["ParticleCount"].as<uint32_t>();
        std::string texturePath = node["TexturePath"].as<std::string>();

        ParticleSystemComponent& particleSystemComponent = texturePath == "" ? entity.AddComponent<ParticleSystemComponent>(particleCount) : entity.AddComponent<ParticleSystemComponent>(particleCount, texturePath);

        particleSystemComponent.Data.BurstSize = node["BurstSize"].as<int>();
        particleSystemComponent.Data.ColorBegin = node["ColorBegin"].as<glm::vec4>();
        particleSystemComponent.Data.ColorEnd = node["ColorEnd"].as<glm::vec4>();
        particleSystemComponent.Data.Lifetime = node["Lifetime"].as<float>();
        particleSystemComponent.Data.LifetimeVariation = node["LifetimeVariation"].as<float>();
        particleSystemComponent.Data.Position = node["Position"].as<glm::vec3>();
        particleSystemComponent.Data.SizeBegin = node["SizeBegin"].as<float>();
        particleSystemComponent.Data.SizeEnd = node["SizeEnd"].as<float>();
        particleSystemComponent.Data.SizeVariation = node["SizeVariation"].as<float>();
        particleSystemComponent.Data.Velocity = node["Velocity"].as<glm::vec3>();
        particleSystemComponent.Data.VelocityVariation = node["VelocityVariation"].as<glm::vec3>();
    }
}