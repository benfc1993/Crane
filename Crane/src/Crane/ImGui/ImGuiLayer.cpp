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
        // ImVec4 darkGrey = { 0.3f, 0.3f, 0.31f, 1.0f };
        // ImVec4 midGrey = { 0.38f, 0.38f, 0.381f, 1.0f };
        // ImVec4 grey = { 0.2f, 0.2f, 0.21f, 1.0f };
        // ImVec4 lightgrey = { 0.2f, 0.2f, 0.21f, 1.0f };
        // ImVec4 brightGrey = { 0.15f, 0.15f, 0.16f, 1.0f };

        // ImVec4 primary = { 0.23f, 0.95f, 1.0f, 1.0f }; //#3BF3FF
        // ImVec4 white = { 0.98, 1.0f, 1.0f, 1.0f }; //#F9FFFF
        // ImVec4 darkGrey = { 0.22f, 0.28f, 0.36f, 1.0f }; //#38475C
        // ImVec4 grey = { 0.25f, 0.31f, 0.39f, 1.0f }; //#414F63
        // ImVec4 lightGrey = { 0.36f, 0.45f, 0.55f, 1.0f }; //#5C738D

        colors[ImGuiCol_WindowBg] = m_Theme.WindowBg;

        // Headers
        colors[ImGuiCol_Header] = m_Theme.Medium;
        colors[ImGuiCol_HeaderHovered] = m_Theme.Dark;
        colors[ImGuiCol_HeaderActive] = m_Theme.Primary;

        // Buttons
        colors[ImGuiCol_Button] = m_Theme.Medium;
        colors[ImGuiCol_ButtonHovered] = m_Theme.Dark;
        colors[ImGuiCol_ButtonActive] = m_Theme.Light;

        // Frame BG
        colors[ImGuiCol_FrameBg] = m_Theme.Medium;
        colors[ImGuiCol_FrameBgHovered] = m_Theme.Dark;
        colors[ImGuiCol_FrameBgActive] = m_Theme.Primary;

        // Tabs
        colors[ImGuiCol_Tab] = m_Theme.Light;
        colors[ImGuiCol_TabHovered] = m_Theme.Medium;
        colors[ImGuiCol_TabActive] = m_Theme.Medium;
        colors[ImGuiCol_TabUnfocused] = m_Theme.Light;
        colors[ImGuiCol_TabUnfocusedActive] = m_Theme.Medium;

        // Title
        colors[ImGuiCol_TitleBg] = m_Theme.Light;
        colors[ImGuiCol_TitleBgActive] = m_Theme.Light;
        colors[ImGuiCol_TitleBgCollapsed] = m_Theme.Light;

    }
}
