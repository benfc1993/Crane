#pragma once

#include "Crane/Renderer/Shader/Texture.h"

#include <imgui/imgui.h>

namespace Crane {
    namespace Drawers {
        void TextureDrawer(Ref<Texture2D>& texture, std::filesystem::path assetPath);

        template<typename T>
        void StyledButton(ImVec4& color, T button)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, color);

            float hoverScaler = 0.8f;
            ImVec4 hoverColor = { color.x * hoverScaler, color.y * hoverScaler, color.z * hoverScaler, color.w * hoverScaler };
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);

            float activeScaler = 0.7f;
            ImVec4 activeColor = { color.x * activeScaler, color.y * activeScaler, color.z * activeScaler, color.w * activeScaler };
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);

            button();


            ImGui::PopStyleColor(3);
        }

    }
}