#include "SceneToolbar.h"

#include <imgui/imgui.h>

namespace Crane {
    void SceneToolbar::OnImGuiRender()
    {
        SceneState SceneState = m_ActiveScene->GetState();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);


        Ref<Texture2D> runtimeIcon = SceneState == SceneState::Play ? m_StopIcon : m_PlayIcon;
        float size = ImGui::GetWindowHeight() - 20.0f;
        ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosY((ImGui::GetContentRegionMax().y * 0.5f) - (size * 0.5f));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
        if (SceneState == SceneState::Simulate) ImGui::BeginDisabled();
        if (ImGui::ImageButton("##runtime", (ImTextureID)runtimeIcon->GetRendererId(), ImVec2(size, size)))
        {
            if (SceneState == SceneState::Edit)
                OnPlay();
            if (SceneState == SceneState::Play)
                OnStop();
        }

        if (SceneState == SceneState::Simulate) ImGui::EndDisabled();

        ImGui::SameLine();

        Ref<Texture2D> simulationIcon = SceneState == SceneState::Simulate ? m_StopIcon : m_SimulateIcon;

        if (SceneState == SceneState::Play) ImGui::BeginDisabled();
        if (ImGui::ImageButton("##simulate", (ImTextureID)simulationIcon->GetRendererId(), ImVec2(size, size)))
        {
            if (SceneState == SceneState::Edit)
                OnSimulateStart();
            if (SceneState == SceneState::Simulate)
                OnSimulateStop();
        }
        if (SceneState == SceneState::Play) ImGui::EndDisabled();

        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::SetCursorPosY((ImGui::GetContentRegionMax().y * 0.5f) - (size * 0.5f));
        ImGui::Checkbox("Show Colliders", &m_EditorSettings->Get()->ShowColiders);

        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}