#include "ParticleSystemPropertiesPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

namespace Crane {
    void ParticleSystemPropertiesPanel::OnImGuiRender()
    {
        ImGui::Begin("Particle System Propertis");
        ImGui::ColorEdit4("Start Color", glm::value_ptr(m_ParticleData->ColorBegin));
        ImGui::ColorEdit4("End Color", glm::value_ptr(m_ParticleData->ColorEnd));

        ImGui::DragFloat("Life Time", &m_ParticleData->Lifetime, 0.1f, 0.0f, 1000.0f);
        ImGui::DragFloat("Life Time Variation", &m_ParticleData->LifetimeVariation, 0.1f, 0.0f, 1.0f);

        ImGui::InputFloat("Initial Size", &m_ParticleData->SizeBegin);
        ImGui::InputFloat("End Size", &m_ParticleData->SizeEnd);
        ImGui::DragFloat("Size Variation", &m_ParticleData->SizeVariation, 0.1f, 0.0f, 1.0f);

        ImGui::InputFloat2("Initial Velocity", glm::value_ptr(m_ParticleData->Velocity));
        ImGui::DragFloat2("Velocity Variation", glm::value_ptr(m_ParticleData->VelocityVariation), 0.1f, 0.0f, 1.0f);

        ImGui::InputInt("Burst Size", &m_ParticleData->BurstSize);
        ImGui::End();

    }
}

