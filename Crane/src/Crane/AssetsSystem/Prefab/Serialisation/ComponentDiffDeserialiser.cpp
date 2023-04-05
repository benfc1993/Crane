#include "ComponentDiffSerialiser.h"

#include "Crane/Scene/Components.h" 
#include "Crane/Scene/Serialisation/ComponentDeserialiser.h" 
#include "Crane/Scripting/ScriptEngine.h"


namespace Crane {

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<TransformComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        value[fieldName] = ValueDiff<glm::vec3>(original, updated, changed);

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<HierarchyComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        value[fieldName] = ValueDiff<uint64_t>(original, updated, changed);

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<PrefabComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        value[fieldName] = ValueDiff<uint64_t>(original, updated, changed);

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CameraComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "ProjectionType")
        {
            value[fieldName] = ValueDiff<int>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<SpriteRendererComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "Color")
        {
            value[fieldName] = ValueDiff<glm::vec4>(original, updated, changed);
        }
        else if (fieldName == "TilingFactor")
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<std::string>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CircleRendererComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "Color")
        {
            value[fieldName] = ValueDiff<glm::vec4>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<RigidBody2DComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "Type")
        {
            value[fieldName] = ValueDiff<std::string>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<bool>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<BoxCollider2DComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "Size" || fieldName == "Offset")
        {
            value[fieldName] = ValueDiff<glm::vec2>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<CircleColliderComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "Offset")
        {
            value[fieldName] = ValueDiff<glm::vec2>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    template <>
    bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<ParticleSystemComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    {
        bool changed = false;
        YAML::Node value;

        if (fieldName == "ParticleCount")
        {
            value[fieldName] = ValueDiff<uint32_t>(original, updated, changed);
        }
        else if (fieldName == "TexturePath")
        {
            value[fieldName] = ValueDiff<std::string>(original, updated, changed);
        }
        else if (fieldName == "BurstSize")
        {
            value[fieldName] = ValueDiff<int>(original, updated, changed);
        }
        else if (fieldName == "ColorBegin" || fieldName == "ColorEnd")
        {
            value[fieldName] = ValueDiff<glm::vec4>(original, updated, changed);
        }
        else if (fieldName == "Position" || fieldName == "Velocity")
        {
            value[fieldName] = ValueDiff<glm::vec3>(original, updated, changed);
        }
        else
        {
            value[fieldName] = ValueDiff<float>(original, updated, changed);
        }

        if (changed)
            componentNode.push_back(value);
        return changed;
    }

    //TODO: Fix script component diff
    //     template <>
    //     bool ComponentDiffDeserialiser::DeserialiseComponentFieldDiff<ScriptComponent>(YAML::Node& componentNode, YAML::Node original, YAML::Node updated, std::string fieldName)
    //     {
    //         bool changed = false;
    //         YAML::Node value;

    //         if (fieldName != "Fields")
    //         {
    //             value[fieldName] = ValueDiff<std::string>(original, updated[fieldName], changed);
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


    //         if (changed)
    //             componentNode.push_back(value);
    //         return changed;
    //     }

    YAML::Node ComponentDiffDeserialiser::DeserialiseComponentDiff(YAML::Node original, YAML::Node updated, std::string componentName, bool& componentChanged)
    {
        YAML::Node componentNode;
        for (auto componentField : updated)
        {
            auto fieldName = componentField.first.as<std::string>();

            if (componentName == "TransformComponent") componentChanged |= DeserialiseComponentFieldDiff<TransformComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "HierarchyComponent") componentChanged |= DeserialiseComponentFieldDiff<HierarchyComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "PrefabComponent") componentChanged |= DeserialiseComponentFieldDiff<PrefabComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "CameraComponent") componentChanged |= DeserialiseComponentFieldDiff<CameraComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "SpriteRendererComponent") componentChanged |= DeserialiseComponentFieldDiff<SpriteRendererComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "CircleRendererComponent") componentChanged |= DeserialiseComponentFieldDiff<CircleRendererComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "RigidBody2DComponent") componentChanged |= DeserialiseComponentFieldDiff<RigidBody2DComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "BoxCollider2DComponent") componentChanged |= DeserialiseComponentFieldDiff<BoxCollider2DComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "CircleColliderComponent") componentChanged |= DeserialiseComponentFieldDiff<CircleColliderComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            if (componentName == "ParticleSystemComponent") componentChanged |= DeserialiseComponentFieldDiff<ParticleSystemComponent>(componentNode, original[fieldName], updated[fieldName], fieldName);
            // if (componentName == "ScriptComponent") componentChanged |= DeserialiseComponentFieldDiff<ScriptComponent>(componentNode, original[fieldName], updated, fieldName);

        }
        return componentNode;
    }
}