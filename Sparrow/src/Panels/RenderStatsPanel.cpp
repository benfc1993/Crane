#include "RenderStatsPanel.h"

#include "Crane.h"

#include "imgui/imgui.h"

namespace Crane {

    void RenderStatsPanel()
    {
        auto stats = Renderer2D::GetStats();

        ImGui::Begin("Render Statistics");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads Drawn: %d", stats.QuadsDrawn);
        ImGui::End();
    }
}