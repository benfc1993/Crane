#include "crpch.h"
#include "ParticleSystem.h"

#include "Crane/Renderer/Renderer2D.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

namespace Crane {
    ParticleSystem::ParticleSystem(u_int32_t particleCount)
    {
        m_ParticlePool.resize(particleCount);
        m_PoolIndex = particleCount - 1;
    }

    void ParticleSystem::OnUpdate(Time time)
    {
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
            particle.Position += particle.Velocity * (float)time.DeltaTime();
            particle.Rotation += 0.01f * time.DeltaTime();
        }
    }

    void ParticleSystem::OnRender(OrthographicCamera& camera)
    {
        for (auto& particle : m_ParticlePool)
        {
            if (!particle.Active)
                continue;
            float life = particle.LifeRemaining / particle.LifeTime;
            glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

            float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

            Renderer2D::DrawQuad(particle.Position, particle.Rotation, { size, size }, particle.Texture, color);
        }
    }

    void ParticleSystem::Emit(const ParticleData& particleData)
    {
        Particle& particle = m_ParticlePool[m_PoolIndex];

        particle.Active = true;
        particle.Position = particleData.Position;
        particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();
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