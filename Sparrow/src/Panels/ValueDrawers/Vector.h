#pragma once

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <string>
#include <imgui/imgui_internal.h>

#include "../UIComponents/Buttons.h"  

namespace Crane {
    namespace Drawers
    {
        struct VectorOptions
        {
            float resetValue = 0.0f;
            float step = 1.0f;
            float min = 0.0f;
            float max = 0.0f;
            std::string fmt = "%.2f";
            VectorOptions() = default;
        };

        template<typename T>
        void Vector(const std::string& label, T& vector, VectorOptions options = VectorOptions())
        {
            ImGuiIO& io = ImGui::GetIO();
            int fontCount = io.Fonts->Fonts.Size;
            auto boldFont = io.Fonts->Fonts[fontCount - 1];

            ImGui::PushID(label.c_str());

            ImGui::Text("%s", label.c_str());


            int dimensions = vector.length();
            std::string labels[] = { "X", "Y", "Z", "W" };
            ImVec4 colors[] = {
                {0.86f, 0.29f, 0.15f, 1.0f},
                {0.63f, 0.86f, 0.18f, 1.0f},
                {0.11f, 0.7f, 0.86f, 1.0f},
                {0.11f, 0.7f, 0.86f, 1.0f}
            };

            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

            ImVec4 color = { 0.0f, 0.0f, 0.0f, 0.0f };


            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 0.5f, lineHeight };

            for (int i = 0; i < dimensions; i++)
            {
                ImGui::PushFont(boldFont);
                StyledButton(colors[i], [&]() {
                    if (ImGui::Button(labels[i].c_str(), buttonSize))
                        vector[i] = options.resetValue;
                });
                ImGui::PopFont();

                std::string label = "##" + labels[i];

                ImGui::SameLine();
                ImGui::DragFloat(label.c_str(), &vector[i], options.step, options.min, options.max, options.fmt.c_str());
                ImGui::PopItemWidth();
                if (i != dimensions - 1)
                    ImGui::SameLine();
            }

            ImGui::PopStyleVar();
            ImGui::Spacing();
            ImGui::PopID();
        }
    }
}