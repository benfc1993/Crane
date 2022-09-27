#pragma once

#include "SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ScriptableEntity.h"

namespace Crane {
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& name)
            : Tag(name)
        {
        }
    };

    struct TransformComponent
    {
        glm::vec3 Position{ 0.0f };
        float Rotation = 0.0f;
        glm::vec2 Scale{ 1.0f };
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform)
        {
        }

        operator glm::mat4& ()
        {
            Transform = glm::translate(glm::mat4(1.0f), Position)
                * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { Scale.x, Scale.y, 1.0f });

            return Transform;
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color)
        {
        }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

    };

    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstatiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstatiateScript = []() { return static_cast<ScriptableEntity*>(new T());};
            DestroyScript = [](NativeScriptComponent* nativeScriptComponent) { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
        }
    };
}