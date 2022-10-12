#include "crpch.h"

#include "ComponentSerializer.h"

#include "Crane/Scene/Components.h"

#include "Crane/Serialization/YamlOperators.h"

namespace Crane {

    template <>
    void ComponentSerializer::SerializeComponent<TagComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {

            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;

            out << YAML::EndMap; // TagComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<TransformComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap; // TransformComponent

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Position" << YAML::Value << tc.Position;
            out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

            out << YAML::EndMap; // TransformComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<CameraComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap; // CameraComponent

            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;

            out << YAML::Key << "Camera" << YAML::Value;
            out << YAML::BeginMap; // Camera
            out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
            out << YAML::Key << "PerspectiveVerticalFov" << YAML::Value << camera.GetPerspectiveVerticalFov();
            out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
            out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
            out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
            out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
            out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
            out << YAML::EndMap; // Camera

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

            out << YAML::EndMap; // CameraComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<SpriteRendererComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<SpriteRendererComponent>())
        {
            out << YAML::Key << "SpriteRendererComponent";
            out << YAML::BeginMap; // SpriteRendererComponent

            auto& sprite = entity.GetComponent<SpriteRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << sprite.Color;
            std::string texturePath = (*sprite.Texture.get());
            out << YAML::Key << "Texture" << YAML::Value << texturePath;

            out << YAML::EndMap; // SpriteRendererComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<ParticleSystemComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<ParticleSystemComponent>())
        {
            out << YAML::Key << "ParticleSystemComponent";
            out << YAML::BeginMap; // ParticleSystemComponent

            auto& particles = entity.GetComponent<ParticleSystemComponent>();
            auto& data = particles.Data;
            auto& system = particles.System;

            out << YAML::Key << "ParticleCount" << YAML::Value << system.GetParticleCount();
            out << YAML::Key << "BurstSize" << YAML::Value << data.BurstSize;
            out << YAML::Key << "ColorBegin" << YAML::Value << data.ColorBegin;
            out << YAML::Key << "ColorEnd" << YAML::Value << data.ColorEnd;
            out << YAML::Key << "Lifetime" << YAML::Value << data.Lifetime;
            out << YAML::Key << "LifetimeVariation" << YAML::Value << data.LifetimeVariation;
            out << YAML::Key << "Position" << YAML::Value << data.Position;
            out << YAML::Key << "SizeBegin" << YAML::Value << data.SizeBegin;
            out << YAML::Key << "SizeEnd" << YAML::Value << data.SizeEnd;
            out << YAML::Key << "SizeVariation" << YAML::Value << data.SizeVariation;
            std::string texturePath = (*data.Texture.get());
            out << YAML::Key << "TexturePath" << YAML::Value << texturePath;
            out << YAML::Key << "Velocity" << YAML::Value << data.Velocity;
            out << YAML::Key << "VelocityVariation" << YAML::Value << data.VelocityVariation;

            out << YAML::EndMap; // ParticleSystemComponent
        }

    }
};