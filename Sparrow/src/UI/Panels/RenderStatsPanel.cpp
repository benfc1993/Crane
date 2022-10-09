#include "RenderStatsPanel.h"

#include "Crane.h"

#include "imgui/imgui.h"

namespace Crane {
    static int frameCount = 1;
    static float frameTime = 0.0f;
    static float avgFrameTime = 0.0f;
    static float fps = 0.0f;

    void RenderStatsPanel::OnImGuiRender()
    {
        auto stats = Renderer2D::GetStats();
        float lastFrameTime = Application::Get().GetLastFrameDuration();

        if (frameCount == 60)
        {
            avgFrameTime = frameTime / (float)frameCount;
            fps = 1.0f / (float)avgFrameTime;
            frameCount = 1;
            frameTime = lastFrameTime;
        }
        else
        {
            frameCount++;
            frameTime += lastFrameTime;
        }

        std::string name = "Render Statistics###" + std::to_string(m_Index);
        ImGui::Begin(name.c_str());
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads Drawn: %d", stats.QuadsDrawn);
        ImGui::Text("Frame time Avg: %0.3f", avgFrameTime);
        ImGui::Text("FPS: %0.1f", fps);
        ImGui::End();
    }
}