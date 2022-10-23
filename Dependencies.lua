IncludeDir = {}
IncludeDir["Box2D"] = "%{wks.location}/Crane/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/Crane/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Crane/vendor/GLAD/include"
IncludeDir["ImGui"] = "%{wks.location}/Crane/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Crane/vendor/glm"
IncludeDir["mono"] = "%{wks.location}/Crane/vendor/mono/include"
IncludeDir["stb_image"] = "%{wks.location}/Crane/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Crane/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Crane/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Crane/vendor/ImGuizmo"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/Crane/vendor/mono/lib"

Library = {}
-- Library["mono"] = "%{LibraryDir.mono}/libmonosgen-2.0.so"

if os.host() == "windows" then

    print("Test")

    VULKAN_SDK = os.getenv(VULKAN_SDK)

    IncludeDir["shaderc"] = "%{wks.location}/Crane/vendor/shaderc/include"
    IncludeDir["SPIRV_Cross"] = "%{wks.location}/Crane/vendor/SPIRV-Cross"
    IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/include"


    LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/lib"
    LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Crane/vendor/VulkanSDK/lib"

    Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/libvulkan.so.1"
    Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/libVkLayer_utils.a"

    Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/libshaderc_shared.so.1"
    Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/libspirv-cross-core.a"
    Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/libspirv-cross-glsl.a"

end