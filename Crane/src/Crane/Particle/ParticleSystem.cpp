#include "crpch.h"
#include "ParticleSystem.h"

#include "Crane/Renderer/Renderer2D.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

namespace Crane {
    ParticleSystem::ParticleSystem()
    {
        m_ParticlePool.resize(m_ParticleCount);
        m_PoolIndex = m_ParticleCount - 1;
    }

    ParticleSystem::ParticleSystem(uint32_t particleCount)
    {
        m_ParticleCount = particleCount;
        m_ParticlePool.resize(particleCount);
        m_PoolIndex = particleCount - 1;
    }

    void ParticleSystem::OnUpdate(Time time)
    {
        CR_PROFILE_FUNCTION();

        if (!m_Active) return;

        if (time.DeltaTime() > 1)
            return;
        for (auto& particle : m_ParticlePool)
        {
            if (!particle.Active)
                continue;


            if (particle.LifeRemaining <= 0.0f)
            {
                particle.Active = false;
                continue;
            }

            particle.LifeRemaining -= time.DeltaTime();
            particle.Position += glm::vec3{ particle.Velocity.x, particle.Velocity.y, 0.0f } *(float)time.DeltaTime();
            particle.Rotation += Random::Float() * 10.0f * time.DeltaTime();
        }
    }

    void ParticleSystem::OnRender()
    {
        CR_PROFILE_FUNCTION();

        if (!m_Active) return;

        for (auto& particle : m_ParticlePool)
        {
            if (!particle.Active)
                continue;
            float life = particle.LifeRemaining / particle.LifeTime;
            glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

            float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

            TextureParameters textureParameters = TextureParameters(particle.Texture);
            textureParameters.Color = color;

            Renderer2D::DrawRotatedQuad(particle.Position, particle.Rotation, { size, size }, textureParameters);
        }
    }

    void ParticleSystem::Emit(const ParticleData& particleData)
    {
        CR_PROFILE_FUNCTION();

        Particle& particle = m_ParticlePool[m_PoolIndex];

        float particleZ = 0.000001f * (float)m_PoolIndex;

        particle.Active = true;
        particle.Position = { particleData.Position.x, particleData.Position.y,  particleZ };
        particle.Rotation = Random::Float() * 60.0f;
        particle.SizeBegin = particleData.SizeBegin + (particleData.SizeBegin * (particleData.SizeVariation * (((float)Random::Float() * 2.0f) - 1.0f)));
        particle.SizeEnd = particleData.SizeEnd;

        particle.ColorBegin = particleData.ColorBegin;
        particle.ColorEnd = particleData.ColorEnd;
        particle.Texture = particleData.Texture;
        particle.LifeTime = particleData.Lifetime + (particleData.Lifetime * (particleData.LifetimeVariation * (((float)Random::Float() * 2.0f) - 1.0f)));
        particle.LifeRemaining = particle.LifeTime;

        particle.Velocity = particleData.Velocity;
        particle.Velocity.x += (particle.Velocity.x * particleData.VelocityVariation.x * (((float)Random::Float() * 2.0f) - 1.0f));
        particle.Velocity.y += (particle.Velocity.y * particleData.VelocityVariation.y * (((float)Random::Float() * 2.0f) - 1.0f));

        m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
    }

}