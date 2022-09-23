#pragma once

#include "Crane/Core/Base.h"
#include "Crane/Camera/OrthographicCamera.h"
#include "Crane/Renderer/Shader/Texture.h"


#include <glm/glm.hpp>

namespace Crane {
    struct ParticleData
    {
        glm::vec2 Position;
        glm::vec2 Velocity, VelocityVariation = { 0.5f, 0.5f };
        glm::vec4 ColorBegin, ColorEnd;
        float SizeBegin, SizeEnd;
        float Lifetime = 1.0f;
        float LifetimeVariation = 0.1f, SizeVariation = 0.3f;
        Ref<Texture2D> Texture;

        int BurstSize = 5;
    };

    class ParticleSystem
    {
    public:
        ParticleSystem(u_int32_t particleCount);
        ~ParticleSystem() {}

        void OnUpdate(Time time);
        void OnRender(OrthographicCamera& camera);

        void Emit(const ParticleData& particleData);
    private:
        struct Particle
        {
            glm::vec2 Position;
            glm::vec2 Velocity;
            glm::vec4 ColorBegin, ColorEnd;
            Ref<Texture2D> Texture;

            float Rotation = 0.0f;
            float SizeBegin, SizeEnd;

            float LifeTime = 1.0f;
            float LifeRemaining = 0.0f;

            bool Active = false;
        };

        std::vector<Particle> m_ParticlePool;
        uint32_t m_PoolIndex;
    };
}