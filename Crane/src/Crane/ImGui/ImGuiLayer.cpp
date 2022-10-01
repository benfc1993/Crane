#include "crpch.h"

#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Crane/Core/Application.h"

// TODO: Remove this
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Crane
{
    ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        CR_PROFILE_FUNCTION();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Light.ttf", 16.0f);
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-Regular.ttf", 16.0f);
        io.Fonts->AddFontFromFileTTF("assets/fonts/Poppins/Poppins-SemiBold.ttf", 16.0f);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
        CR_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        if (m_ShouldBlockEvents)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }

    void ImGuiLayer::SetDarkThemeColors()
    {
        auto& colors = ImGui::GetStyle().Colors;
        ImVec4 darkGrey = { 0.3f, 0.3f, 0.31f, 1.0f };
        ImVec4 midGrey = { 0.38f, 0.38f, 0.381f, 1.0f };
        ImVec4 grey = { 0.2f, 0.2f, 0.21f, 1.0f };
        ImVec4 lightgrey = { 0.2f, 0.2f, 0.21f, 1.0f };
        ImVec4 brightGrey = { 0.15f, 0.15f, 0.16f, 1.0f };

        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = grey;
        colors[ImGuiCol_HeaderHovered] = darkGrey;
        colors[ImGuiCol_HeaderActive] = brightGrey;

        // Buttons
        colors[ImGuiCol_Button] = grey;
        colors[ImGuiCol_ButtonHovered] = darkGrey;
        colors[ImGuiCol_ButtonActive] = brightGrey;

        // Frame BG
        colors[ImGuiCol_FrameBg] = grey;
        colors[ImGuiCol_FrameBgHovered] = darkGrey;
        colors[ImGuiCol_FrameBgActive] = brightGrey;

        // Tabs
        colors[ImGuiCol_Tab] = brightGrey;
        colors[ImGuiCol_TabHovered] = midGrey;
        colors[ImGuiCol_TabActive] = lightgrey;
        colors[ImGuiCol_TabUnfocused] = brightGrey;
        colors[ImGuiCol_TabUnfocusedActive] = grey;

        // Title
        colors[ImGuiCol_TitleBg] = brightGrey;
        colors[ImGuiCol_TitleBgActive] = brightGrey;
        colors[ImGuiCol_TitleBgCollapsed] = brightGrey;
    }
}
