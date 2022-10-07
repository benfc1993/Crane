#include "RenderStatsPanel.h"

#include "Crane.h"

#include "imgui/imgui.h"

namespace Crane {
    static int frameCount = 1;
    static float frameTime = 0.0f;
    void RenderStatsPanel::OnImGuiRender()
    {
        auto stats = Renderer2D::GetStats();
        float lastFrameTime = Application::Get().GetLastFrameDuration();
        float fps = 1.0f / (float)lastFrameTime;

        if (frameCount == 60)
        {
            frameCount = 1;
            frameTime = lastFrameTime;
        }
        else
        {
            frameCount++;
            frameTime += lastFrameTime;
        }

        float avgFrameTime = frameTime / (float)frameCount;

        ImGui::Begin("Render Statistics");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads Drawn: %d", stats.QuadsDrawn);
        ImGui::Text("Frame time Avg: %0.3f", avgFrameTime);
        ImGui::Text("FPS: %0.1f", fps);
        ImGui::End();
    }
}