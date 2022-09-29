#include "ValueDrawers.h"

#include "../UIComponents/Buttons.h"  

#include <glm/glm.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Crane {
    namespace Drawers {

        void Vector3(const std::string& label, glm::vec3& vector, float resetValue, float columnWidth)
        {
            std::string labels[] = { "X", "Y", "Z" };
            ImVec4 colors[] = {
                {0.86f, 0.29f, 0.15f, 1.0f},
                {0.63f, 0.86f, 0.18f, 1.0f},
                {0.11f, 0.7f, 0.86f, 1.0f}
            };

            ImGui::Columns(2);
            ImGui::Text("%s", label.c_str());
            ImGui::SetColumnWidth(0, columnWidth);
            ImGui::NextColumn();

            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

            ImVec4 color = { 0.0f, 0.0f, 0.0f, 0.0f };


            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 0.5f, lineHeight };

            for (int i = 0; i < 3; i++)
            {

                StyledButton(colors[i], [&]() {
                    if (ImGui::Button(labels[i].c_str(), buttonSize))
                        vector[i] = resetValue;
                    });

                std::string label = "##" + labels[i];

                ImGui::SameLine();
                ImGui::DragFloat(label.c_str(), &vector[i]);
                ImGui::PopItemWidth();
                if (i != 2)
                    ImGui::SameLine();
            }

            ImGui::PopStyleVar();
            ImGui::Columns(1);
        }

    }
}