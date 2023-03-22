#include "crpch.h"

#include "ComponentSerializer.h"

#include "Crane/Scene/Components.h"
#include "Crane/Scripting/ScriptEngine.h"

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
    void ComponentSerializer::SerializeComponent<HierarchyComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<HierarchyComponent>())
        {

            out << YAML::Key << "HierarchyComponent";
            out << YAML::BeginMap; // TagComponent

            auto& hc = entity.GetComponent<HierarchyComponent>();
            out << YAML::Key << "Parent" << YAML::Value << hc.Parent;
            out << YAML::Key << "First" << YAML::Value << hc.First;
            out << YAML::Key << "Prev" << YAML::Value << hc.Prev;
            out << YAML::Key << "Next" << YAML::Value << hc.Next;

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
            out << YAML::Key << "TilingFactor" << YAML::Value << sprite.TilingFactor;

            out << YAML::EndMap; // SpriteRendererComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<CircleRendererComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<CircleRendererComponent>())
        {
            out << YAML::Key << "CircleRendererComponent";
            out << YAML::BeginMap; // CircleRendererComponent

            auto& circle = entity.GetComponent<CircleRendererComponent>();
            out << YAML::Key << "Color" << YAML::Value << circle.Color;
            out << YAML::Key << "Thickness" << YAML::Value << circle.Thickness;
            out << YAML::Key << "Fade" << YAML::Value << circle.Fade;

            out << YAML::EndMap; // CircleRendererComponent
        }

    }

    template <>
    void ComponentSerializer::SerializeComponent<RigidBody2DComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<RigidBody2DComponent>())
        {
            out << YAML::Key << "RigidBody2DComponent";
            out << YAML::BeginMap; // RigidBody2DComponent

            auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
            out << YAML::Key << "Type" << YAML::Value << RigidBodyTypeToString(rb2d.Type);
            out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;

            out << YAML::EndMap; // RigidBody2DComponent
        }
    }

    template <>
    void ComponentSerializer::SerializeComponent<BoxCollider2DComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            out << YAML::Key << "BoxCollider2DComponent";
            out << YAML::BeginMap; // BoxCollider2DComponent

            auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();
            out << YAML::Key << "Size" << YAML::Value << boxCollider.Size;
            out << YAML::Key << "Offset" << YAML::Value << boxCollider.Offset;
            out << YAML::Key << "Density" << YAML::Value << boxCollider.Density;
            out << YAML::Key << "Friction" << YAML::Value << boxCollider.Friction;
            out << YAML::Key << "Restitution" << YAML::Value << boxCollider.Restitution;
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider.RestitutionThreshold;

            out << YAML::EndMap; // BoxCollider2DComponent
        }
    }

    template <>
    void ComponentSerializer::SerializeComponent<CircleColliderComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<CircleColliderComponent>())
        {
            out << YAML::Key << "CircleColliderComponent";
            out << YAML::BeginMap; // CircleColliderComponent

            auto& circleCollider = entity.GetComponent<CircleColliderComponent>();
            out << YAML::Key << "Radius" << YAML::Value << circleCollider.Radius;
            out << YAML::Key << "Offset" << YAML::Value << circleCollider.Offset;
            out << YAML::Key << "Density" << YAML::Value << circleCollider.Density;
            out << YAML::Key << "Friction" << YAML::Value << circleCollider.Friction;
            out << YAML::Key << "Restitution" << YAML::Value << circleCollider.Restitution;
            out << YAML::Key << "RestitutionThreshold" << YAML::Value << circleCollider.RestitutionThreshold;

            out << YAML::EndMap; // CircleColliderComponent
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


    template <>
    void ComponentSerializer::SerializeComponent<ScriptComponent>(YAML::Emitter& out, Entity entity)
    {
        if (entity.HasComponent<ScriptComponent>())
        {
            auto& sc = entity.GetComponent<ScriptComponent>();

            out << YAML::Key << "ScriptComponent";
            out << YAML::BeginMap; // ScriptComponent
            out << YAML::Key << "FullName" << YAML::Value << sc.FullName;

            auto& fieldInstances = ScriptEngine::GetScriptFieldMap(entity);
            if (fieldInstances.size() > 0)
            {
                out << YAML::Key << "Fields";
                out << YAML::BeginSeq; // Fields

                for (const auto& [name, fieldInstance] : fieldInstances)
                {
                    out << YAML::BeginMap;
                    out << YAML::Key << "Name" << YAML::Value << name;
                    out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(fieldInstance.Field.Type);
                    out << YAML::Key << "Value" << YAML::Value;

                    ScriptFieldInstance& scriptField = fieldInstances.at(name);

#define FIELD_TYPE(FieldType, Type) case ScriptFieldType::FieldType:\
                    out << scriptField.GetValue<Type>(); \
                    break

                    switch (fieldInstance.Field.Type)
                    {
                        FIELD_TYPE(Float, float);
                        FIELD_TYPE(Double, double);
                        FIELD_TYPE(Bool, bool);
                        FIELD_TYPE(Char, char);
                        FIELD_TYPE(Byte, int8_t);
                        FIELD_TYPE(Short, int16_t);
                        FIELD_TYPE(Int, int32_t);
                        FIELD_TYPE(Long, int64_t);
                        FIELD_TYPE(UByte, uint8_t);
                        FIELD_TYPE(UShort, uint16_t);
                        FIELD_TYPE(UInt, uint32_t);
                        FIELD_TYPE(ULong, uint64_t);
                        FIELD_TYPE(Vector2, glm::vec2);
                        FIELD_TYPE(Vector3, glm::vec3);
                        FIELD_TYPE(Vector4, glm::vec4);
                        FIELD_TYPE(Entity, UUID);
                        FIELD_TYPE(Script, UUID);
                    }
#undef FIELD_TYPE
                    out << YAML::EndMap;
                }

                out << YAML::EndSeq; // Fields
            }


            out << YAML::EndMap; // ScriptComponent
        }
    }

    template <>
    void ComponentSerializer::SerializeComponent<NativeScriptComponent>(YAML::Emitter& out, Entity entity)
    {
        return;
    }
};