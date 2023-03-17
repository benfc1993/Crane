#include "crpch.h"

#include "ImGuiLayer.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Crane/Core/Application.h"

// TODO: Remove this
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <ImGuizmo.h>

namespace Crane
{
        ImGuiLayer::ImGuiLayer(): Layer("ImGui Layer")
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
                        // event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard; // IMGUI don't block keyboard events
                        event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
                }
        }

        void ImGuiLayer::Begin()
        {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                ImGuizmo::BeginFrame();
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
        }

        ImVec4 glmVec4ToImVec4(glm::vec4 value)
        {
                return ImVec4(value.x, value.y, value.z, value.w);
        }

        void ImGuiLayer::SetThemeColors(ImGuiTheme& theme)
        {
                auto& colors = ImGui::GetStyle().Colors;
                ImVec4 Primary = glmVec4ToImVec4(theme.Primary);
                ImVec4 PrimaryLight = glmVec4ToImVec4(theme.PrimaryLight);
                ImVec4 PrimaryDark = glmVec4ToImVec4(theme.PrimaryDark);
                ImVec4 Secondary = glmVec4ToImVec4(theme.Secondary);
                ImVec4 White = glmVec4ToImVec4(theme.White);
                ImVec4 Dark = glmVec4ToImVec4(theme.Dark);
                ImVec4 Medium = glmVec4ToImVec4(theme.Medium);
                ImVec4 Light = glmVec4ToImVec4(theme.Light);
                ImVec4 WindowBg = glmVec4ToImVec4(theme.WindowBg);

#pragma region Text
                colors[ImGuiCol_Text] = White;
                colors[ImGuiCol_TextDisabled] = Light;
#pragma endregion

#pragma region Windows
                colors[ImGuiCol_WindowBg] = WindowBg;
                colors[ImGuiCol_ChildBg] = WindowBg;
                colors[ImGuiCol_PopupBg] = WindowBg;
#pragma endregion

#pragma region Borders
                colors[ImGuiCol_Border] = PrimaryDark;
                colors[ImGuiCol_BorderShadow] = PrimaryDark;
#pragma endregion

#pragma region Frame BG
                colors[ImGuiCol_FrameBg] = Medium;
                colors[ImGuiCol_FrameBgHovered] = Dark;
                colors[ImGuiCol_FrameBgActive] = Dark;
#pragma endregion

#pragma region Title
                colors[ImGuiCol_TitleBg] = Light;
                colors[ImGuiCol_TitleBgActive] = PrimaryDark;
                colors[ImGuiCol_TitleBgCollapsed] = Medium;
#pragma endregion

#pragma region Menu Bar
                colors[ImGuiCol_MenuBarBg] = Dark;
#pragma endregion

#pragma region Scrollbar
                colors[ImGuiCol_ScrollbarBg] = Dark;
                colors[ImGuiCol_ScrollbarGrab] = Light;
                colors[ImGuiCol_ScrollbarGrabHovered] = Primary;
                colors[ImGuiCol_ScrollbarGrabActive] = Light;
#pragma endregion

#pragma region Inputs

                colors[ImGuiCol_CheckMark] = Primary;

                // Sliders
                colors[ImGuiCol_SliderGrab] = Light;
                colors[ImGuiCol_SliderGrabActive] = PrimaryDark;

                // Buttons
                colors[ImGuiCol_Button] = Primary;
                colors[ImGuiCol_ButtonHovered] = PrimaryLight;
                colors[ImGuiCol_ButtonActive] = PrimaryDark;
#pragma endregion

#pragma region Headers
                colors[ImGuiCol_Header] = PrimaryDark;
                colors[ImGuiCol_HeaderHovered] = PrimaryDark;
                colors[ImGuiCol_HeaderActive] = Light;
#pragma endregion

#pragma region Separators
                colors[ImGuiCol_Separator] = Primary;
                colors[ImGuiCol_SeparatorHovered] = PrimaryDark;
                colors[ImGuiCol_SeparatorActive] = PrimaryLight;
#pragma endregion

#pragma region Resize
                colors[ImGuiCol_ResizeGrip] = Primary;            // Resize grip in lower-right and lower-left corners of windows.
                colors[ImGuiCol_ResizeGripHovered] = PrimaryDark;
                colors[ImGuiCol_ResizeGripActive] = PrimaryLight;
#pragma endregion

#pragma region Tabs        
                colors[ImGuiCol_Tab] = Light;
                colors[ImGuiCol_TabHovered] = PrimaryLight;
                colors[ImGuiCol_TabActive] = Primary;
                colors[ImGuiCol_TabUnfocused] = Light;
                colors[ImGuiCol_TabUnfocusedActive] = PrimaryDark;
#pragma endregion

#pragma region Docking
                colors[ImGuiCol_DockingPreview] = PrimaryDark;        // Preview overlay color when about to docking something
                colors[ImGuiCol_DockingEmptyBg] = WindowBg;
#pragma endregion
        }
}
