#include "crpch.h"

#include "ComponentDeserializer.h"

#include "Crane/Scene/Components.h"
#include "Crane/Core/UUID.h"
#include "Crane/Scripting/ScriptEngine.h"

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
    void ComponentDeserializer::DeserializeComponent<HierarchyComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["HierarchyComponent"];
        if (!node) return;

        auto& hc = entity.AddOrReplaceComponent<HierarchyComponent>();
        if (node["Parent"])
            hc.Parent = UUID(node["Parent"].as<uint64_t>());
        if (node["First"])
            hc.First = UUID(node["First"].as<uint64_t>());
        if (node["Prev"])
            hc.Prev = UUID(node["Prev"].as<uint64_t>());
        if (node["Next"])
            hc.Next = UUID(node["Next"].as<uint64_t>());
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
        std::string texturePath = node["Texture"].as<std::string>();
        std::string texture = node["Texture"].as<std::string>();
        if (texture != "")
            spriteRendererComponent.Texture = Texture2D::Create(texture);
        spriteRendererComponent.TilingFactor = node["TilingFactor"].as<float>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<CircleRendererComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["CircleRendererComponent"];
        if (!node) return;

        auto& circleRendererComponent = entity.AddComponent<CircleRendererComponent>();

        circleRendererComponent.Color = node["Color"].as<glm::vec4>();
        circleRendererComponent.Thickness = node["Thickness"].as<float>();
        circleRendererComponent.Fade = node["Fade"].as<float>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<RigidBody2DComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["RigidBody2DComponent"];
        if (!node) return;

        auto& rb2d = entity.AddComponent <RigidBody2DComponent>();

        rb2d.Type = StringToRigidBodyType(node["Type"].as<std::string>());
        rb2d.FixedRotation = node["FixedRotation"].as<bool>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<BoxCollider2DComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["BoxCollider2DComponent"];
        if (!node) return;

        auto& boxCollider = entity.AddComponent <BoxCollider2DComponent>();

        boxCollider.Size = node["Size"].as<glm::vec2>();
        boxCollider.Offset = node["Offset"].as<glm::vec2>();
        boxCollider.Density = node["Density"].as<float>();
        boxCollider.Friction = node["Friction"].as<float>();
        boxCollider.Restitution = node["Restitution"].as<float>();
        boxCollider.RestitutionThreshold = node["RestitutionThreshold"].as<float>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<CircleColliderComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["CircleColliderComponent"];
        if (!node) return;

        auto& circleCollider = entity.AddComponent <CircleColliderComponent>();

        circleCollider.Radius = node["Radius"].as<float>();
        circleCollider.Offset = node["Offset"].as<glm::vec2>();
        circleCollider.Density = node["Density"].as<float>();
        circleCollider.Friction = node["Friction"].as<float>();
        circleCollider.Restitution = node["Restitution"].as<float>();
        circleCollider.RestitutionThreshold = node["RestitutionThreshold"].as<float>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<ParticleSystemComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["ParticleSystemComponent"];
        if (!node) return;

        float particleCount = node["ParticleCount"].as<uint32_t>();
        std::string texturePath = node["TexturePath"].as<std::string>();

        ParticleSystemComponent& particleSystemComponent = texturePath == "" ? entity.AddComponent<ParticleSystemComponent>(particleCount, (int)entity) : entity.AddComponent<ParticleSystemComponent>(particleCount, texturePath, (int)entity);

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
        particleSystemComponent.Data.VelocityVariation = node["VelocityVariation"].as<float>();
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<ScriptComponent>(YAML::Node& data, Entity& entity)
    {
        YAML::Node node = data["ScriptComponent"];
        if (!node) return;
        auto& sc = entity.AddComponent<ScriptComponent>(node["FullName"].as<std::string>());

        auto fields = node["Fields"];
        if (fields)
        {
            Ref<ScriptClass> scriptClass = ScriptEngine::GetScript(sc.FullName);
            auto& fieldInstances = ScriptEngine::GetScriptFieldMap(entity);
            const auto& scriptFields = scriptClass->GetFields();


            for (auto field : fields)
            {
                std::string name = field["Name"].as <std::string>();
                ScriptFieldInstance& scriptFieldInstance = fieldInstances[name];
                scriptFieldInstance.Field = scriptFields.at(name);

                ScriptFieldType type = Utils::StringToScriptFieldType(field["Type"].as<std::string>());

#define FIELD_TYPE(FieldType, Type) case ScriptFieldType::FieldType: \
                scriptFieldInstance.SetValue(field["Value"].as<Type>()); \
                break

                switch (type)
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
                    FIELD_TYPE(Entity, int64_t);
                }
            }

        }

        return;
    }

    template <>
    void ComponentDeserializer::DeserializeComponent<NativeScriptComponent>(YAML::Node& data, Entity& entity)
    {
        return;
    }
}