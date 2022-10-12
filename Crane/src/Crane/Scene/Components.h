#pragma once

#include "SceneCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "ScriptableEntity.h"
#include "Crane/Particle/ParticleSystem.h"
#include "Crane/Renderer/Shader/Texture.h"

namespace Crane
{
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
        glm::vec3 Rotation{ 0.0f };
        glm::vec3 Scale{ 1.0f };
        glm::mat4 Transform{ 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform)
            : Transform(transform)
        {
        }

        operator glm::mat4& ()
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            Transform = glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);

            return Transform;
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color;
        Ref<Texture2D> Texture = Texture2D::Create(1, 1);

        SpriteRendererComponent()
        {
            uint32_t whiteTextureData = 0xffffffff;
            Texture->SetData(&whiteTextureData, sizeof(uint32_t));
        };
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

        template <typename T>
        void Bind()
        {
            InstatiateScript = []()
            { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nativeScriptComponent)
            { delete nativeScriptComponent->Instance; nativeScriptComponent->Instance = nullptr; };
        }
    };

    struct ParticleSystemComponent
    {
        ParticleSystem System;
        ParticleData Data;

        ParticleSystemComponent(int entityId)
        {
            System.SetActive(true);
            Data.EntityId = entityId;
        };
        ParticleSystemComponent(const ParticleSystemComponent&) = default;
        ParticleSystemComponent(const uint32_t particleCount, int entityId)
        {
            System.SetParticleCount(particleCount);
            System.SetActive(true);
            Data.EntityId = entityId;
        }
        ParticleSystemComponent(const uint32_t particleCount, std::string path, int entityId)
        {
            System.SetParticleCount(particleCount);
            System.SetActive(true);
            Data = ParticleData(path);
            Data.EntityId = entityId;
        }
    };
}