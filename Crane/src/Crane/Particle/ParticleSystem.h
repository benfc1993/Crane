#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Camera/OrthographicCamera.h"
#include "Crane/Renderer/Shader/Texture.h"


#include <glm/glm.hpp>

namespace Crane {
    struct ParticleData
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Velocity = { 0.420f, 0.370f, 0.0f }, VelocityVariation = { 0.5f, 0.5f, 0.0f };
        glm::vec4 ColorBegin = { 0.8f, 0.2f, 0.3f, 1.0f };
        glm::vec4 ColorEnd = { 0.036f, 0.044f, 0.054f, 0.059f };
        float SizeBegin = 0.2f, SizeEnd = 0.75f;
        float Lifetime = 1.0f;
        float LifetimeVariation = 0.1f, SizeVariation = 0.3f;
        Ref<Texture2D> Texture = Texture2D::Create(1, 1);
        int BurstSize = 5;
        int EntityId = -1;

        ParticleData()
        {
            uint32_t whiteTextureData = 0xffffffff;
            Texture->SetData(&whiteTextureData, sizeof(uint32_t));
        }

        ParticleData(std::string path)
        {
            Texture = Texture2D::Create(path);
        }
    };

    class ParticleSystem
    {
    public:
        ParticleSystem();
        ParticleSystem(uint32_t particleCount);
        ~ParticleSystem() {}

        void SetParticleCount(uint32_t particleCount)
        {
            m_ParticleCount = particleCount;
            m_ParticlePool.resize(particleCount);
            m_PoolIndex = particleCount - 1;
        }

        uint32_t GetParticleCount() const { return m_ParticleCount; }

        void SetActive(bool isActive) { m_Active = isActive; Reset(); }
        bool GetState() const { return m_Active; }

        void Reset();

        void OnUpdate(Time time);
        void OnRender();

        void Emit(const ParticleData& particleData);
    private:
        struct Particle
        {
            glm::vec3 Position;
            glm::vec2 Velocity;
            glm::vec4 ColorBegin, ColorEnd;
            Ref<Texture2D> Texture;

            float Rotation = 0.0f;
            float SizeBegin, SizeEnd;

            float LifeTime = 1.0f;
            float LifeRemaining = 0.0f;

            bool Active = false;

            int EntityId = -1;
        };

        uint32_t m_ParticleCount = 10000;
        std::vector<Particle> m_ParticlePool;
        uint32_t m_PoolIndex;
        bool m_Active = false;
    };
}