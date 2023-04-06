#include "ComponentDiffSerialiser.h"

#include "Crane/Scene/Components.h" 
#include "Crane/Scene/Serialisation/ComponentDeserialiser.h" 
#include "Crane/Scripting/ScriptEngine.h"


namespace Crane {
#define VALUE_DIFF_ALL(ValueType) return ValueDiff<ValueType>(original, updated, changed);
#define VALUE_DIFF(FieldName, ValueType) if (fieldName == FieldName) return ValueDiff<ValueType>(original, updated, changed);

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<TransformComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Rotation", glm::vec3);
        VALUE_DIFF_ALL(glm::vec3);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<HierarchyComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF_ALL(uint64_t);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<PrefabComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF_ALL(uint64_t);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CameraComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("ProjectionType", int);
        VALUE_DIFF_ALL(float);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<SpriteRendererComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Color", glm::vec4);
        VALUE_DIFF("TilingFactor", float);

        return ValueDiff<std::string>(original, updated, changed);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CircleRendererComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Color", glm::vec4);

        VALUE_DIFF_ALL(float);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<RigidBody2DComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Type", std::string);
        VALUE_DIFF("FixedRotation", bool);
        return YAML::Node;
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<BoxCollider2DComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Size", glm::vec2);
        VALUE_DIFF("Offset", glm::vec2);
        VALUE_DIFF_ALL(float);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CircleColliderComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("Offset", glm::vec2);
        VALUE_DIFF_ALL(float);
    }

    template <>
    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<ParticleSystemComponent>(YAML::Node original, YAML::Node updated, std::string fieldName, bool& changed)
    {
        VALUE_DIFF("ParticleCount", uint32_t);
        VALUE_DIFF("TexturePath", std::string);
        VALUE_DIFF("BurstSize", int);
        VALUE_DIFF("ColorBegin", glm::vec4);
        VALUE_DIFF("ColorEnd", glm::vec4);
        VALUE_DIFF("Position", glm::vec3);
        VALUE_DIFF("Velocity", glm::vec3);
        VALUE_DIFF_ALL(float);
    }

    //TODO: Fix script component diff
    //     template <>
    //     YAML::Node ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<ScriptComponent>(YAML::Node original, YAML::Node updated, std::string fieldName)
    //     {
    //         if (fieldName != "Fields")
    //         {
    //             return ValueDiff<std::string>(original, updated[fieldName], changed);
    //         }
    //         else
    //         {
    //             for (auto field : updated["Fields"])
    //             {
    //                 ScriptFieldType type = Utils::StringToScriptFieldType(field["Type"].as<std::string>());
    //                 YAML::Node scriptFieldNode;

    //                 scriptFieldNode["Name"] = ValueDiff<std::string>(original, field["Name"], changed);
    //                 scriptFieldNode["Type"] = ValueDiff<std::string>(original, field["Type"], changed);

    // #define FIELD_TYPE(FieldType, Type) case ScriptFieldType::FieldType: \ 
    //                 scriptFieldNode["Value"] = ValueDiff<Type>(original, field["Value"], changed); \
            //                     break

            //                     switch (type)
            //                     {
            //                         FIELD_TYPE(Float, float);
            //                         FIELD_TYPE(Double, double);
            //                         FIELD_TYPE(Bool, bool);
            //                         FIELD_TYPE(Char, char);
            //                         FIELD_TYPE(Byte, int8_t);
            //                         FIELD_TYPE(Short, int16_t);
            //                         FIELD_TYPE(Int, int32_t);
            //                         FIELD_TYPE(Long, int64_t);
            //                         FIELD_TYPE(UByte, uint8_t);
            //                         FIELD_TYPE(UShort, uint16_t);
            //                         FIELD_TYPE(UInt, uint32_t);
            //                         FIELD_TYPE(ULong, uint64_t);
            //                         FIELD_TYPE(Vector2, glm::vec2);
            //                         FIELD_TYPE(Vector3, glm::vec3);
            //                         FIELD_TYPE(Vector4, glm::vec4);
            //                         FIELD_TYPE(Entity, uint64_t);
            //                         FIELD_TYPE(Script, uint64_t);
            //                     }

            //                 value["Fields"].push_back(scriptFieldNode);

            //             }
            //         }
            //     }

    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentDiff(YAML::Node original, YAML::Node updated, std::string componentName, bool& componentChanged)
    {
        YAML::Node componentNode;
        for (auto componentField : updated)
        {
            auto fieldName = componentField.first.as<std::string>();

            bool changed = false;
            YAML::Node value;

            if (componentName == "TransformComponent") value = DeserialiseComponentFieldDiff<TransformComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "HierarchyComponent") value = DeserialiseComponentFieldDiff<HierarchyComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "PrefabComponent") value = DeserialiseComponentFieldDiff<PrefabComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "CameraComponent") value = DeserialiseComponentFieldDiff<CameraComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "SpriteRendererComponent") value = DeserialiseComponentFieldDiff<SpriteRendererComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "CircleRendererComponent") value = DeserialiseComponentFieldDiff<CircleRendererComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "RigidBody2DComponent") value = DeserialiseComponentFieldDiff<RigidBody2DComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "BoxCollider2DComponent") value = DeserialiseComponentFieldDiff<BoxCollider2DComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "CircleColliderComponent") value = DeserialiseComponentFieldDiff<CircleColliderComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            if (componentName == "ParticleSystemComponent") value = DeserialiseComponentFieldDiff<ParticleSystemComponent>(original[fieldName], updated[fieldName], fieldName, changed);
            // if (componentName == "ScriptComponent") value = DeserialiseComponentFieldDiff<ScriptComponent>(original[fieldName], updated, fieldName, changed);

            if (changed)
                componentNode[fieldName] = value;

            componentChanged |= changed;
        }
        return componentNode;
    }
}