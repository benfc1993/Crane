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

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;

        glm::mat4 Transform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);


        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color = { 1.0f, 1.0f , 1.0f , 1.0f };
        Ref<Texture2D> Texture = Texture2D::Create(1, 1);
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color)
        {
        }

        void ClearTexture()
        {
            Texture = Texture2D::Create(1, 1);
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
            Data.EntityId = entityId;
        };
        ParticleSystemComponent(const ParticleSystemComponent&) = default;
        ParticleSystemComponent(const uint32_t particleCount, int entityId)
        {
            System.SetParticleCount(particleCount);
            Data.EntityId = entityId;
        }
        ParticleSystemComponent(const uint32_t particleCount, std::string path, int entityId)
        {
            System.SetParticleCount(particleCount);
            Data = ParticleData(path);
            Data.EntityId = entityId;
        }
    };

    //Physics

    struct RigidBody2DComponent
    {
        enum class BodyType { Static, Dynamic, Kinematic };
        BodyType Type = BodyType::Static;
        bool FixedRotation = false;

        void* RuntimeBody = nullptr;

        RigidBody2DComponent() = default;
        RigidBody2DComponent(const RigidBody2DComponent&) = default;
    };

    struct BoxCollider2DComponent
    {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };

        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;

        void* RuntimeCollider = nullptr;

        BoxCollider2DComponent() = default;
        BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
    };
}