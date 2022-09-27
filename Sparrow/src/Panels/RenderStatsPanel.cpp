#include "RenderStatsPanel.h"

#include "Crane.h"

#include "imgui/imgui.h"

namespace Crane {

    void RenderStatsPanel::OnImGuiRender()
    {
        auto stats = Renderer2D::GetStats();
        float lastFrameTime = Application::Get().GetLastFrameDuration();
        float fps = (float)60.0f / lastFrameTime;

        ImGui::Begin("Render Statistics");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads Drawn: %d", stats.QuadsDrawn);
        ImGui::Text("FPS: %f", fps);
        ImGui::End();
    }
}