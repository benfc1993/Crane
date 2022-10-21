#include "SceneToolbar.h"

#include <imgui/imgui.h>

namespace Crane {
    void SceneToolbar::OnImGuiRender()
    {
        SceneState SceneState = m_ActiveScene->GetState();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 2.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

        ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);


        Ref<Texture2D> icon = SceneState == SceneState::Edit ? m_PlayIcon : m_StopIcon;
        float size = ImGui::GetWindowHeight() - 20.0f;
        ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
        ImGui::SetCursorPosY((ImGui::GetContentRegionMax().y * 0.5f) - (size * 0.5f));

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
        if (ImGui::ImageButton("##", (ImTextureID)icon->GetRendererId(), ImVec2(size, size)))
        {
            if (SceneState == SceneState::Edit)
                // m_ActiveScene->SetState(SceneState::Play);
                OnPlay();
            if (SceneState == SceneState::Play)
                // m_ActiveScene->SetState(SceneState::Edit);
                OnStop();


        }
        ImGui::PopStyleColor();

        ImGui::SameLine();

        ImGui::SetCursorPosY((ImGui::GetContentRegionMax().y * 0.5f) - (size * 0.5f));
        ImGui::Checkbox("Show Colliders", &m_EditorSettings->ShowColiders);

        ImGui::End();
        ImGui::PopStyleVar(2);
    }
}