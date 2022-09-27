#pragma once

#include "Crane.h"

namespace Crane {
    class ParticleSystemPropertiesPanel
    {
    public:
        ParticleSystemPropertiesPanel() = default;
        ParticleSystemPropertiesPanel(ParticleData* particleData)
            : m_ParticleData(particleData)
        {
        }

        void SetParticleData(ParticleData* particleData) { m_ParticleData = particleData; }

        void OnImGuiRender();

    private:
        ParticleData* m_ParticleData;
    };
}