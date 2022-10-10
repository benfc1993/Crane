project "Crane"
	language "C++"
	cppdialect "C++17"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crpch.h"
	pchsource "Crane/src/crpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/entt/include/**.hpp",
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
    	"%{IncludeDir.VulkanSDK}",
	}

	links 
	{ 
		"GLFW",
		"GLAD",
    	"ImGui",
    	"yaml-cpp",
	}

	
	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH"}

	filter "system:linux"
		kind "SharedLib"
		systemversion "latest"
		pic "On"
		staticruntime "off"
		
		
	filter "system:windows"
		systemversion "latest"
		kind "StaticLib"
		staticruntime "on"

		defines
		{
			"CR_PLATFORM_WINDOWS",
			"CR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines
        {
            "CR_DEBUG",
        }
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CR_DIST"
		runtime "Release"
		optimize "on"


