#include "ParticleSystemPropertiesPanel.h"

#include <glm/gtc/type_ptr.hpp>

namespace Crane {
    void ParticleSystemPropertiesPanel(ParticleData& particleData)
    {
        ImGui::Begin("Particle System Propertis");
        ImGui::ColorEdit4("Start Color", glm::value_ptr(particleData.ColorBegin));
        ImGui::ColorEdit4("End Color", glm::value_ptr(particleData.ColorEnd));

        ImGui::DragFloat("Life Time", &particleData.Lifetime, 0.1f, 0.0f, 1000.0f);
        ImGui::DragFloat("Life Time Variation", &particleData.LifetimeVariation, 0.1f, 0.0f, 1.0f);

        ImGui::InputFloat("Initial Size", &particleData.SizeBegin);
        ImGui::InputFloat("End Size", &particleData.SizeEnd);
        ImGui::DragFloat("Size Variation", &particleData.SizeVariation, 0.1f, 0.0f, 1.0f);

        ImGui::InputFloat2("Initial Velocity", glm::value_ptr(particleData.Velocity));
        ImGui::DragFloat2("Velocity Variation", glm::value_ptr(particleData.VelocityVariation), 0.1f, 0.0f, 1.0f);

        ImGui::InputInt("Burst Size", &particleData.BurstSize);
        ImGui::End();

    }
}

