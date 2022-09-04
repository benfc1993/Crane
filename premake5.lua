require "vscode"

workspace "Crane"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Crane/vendor/GLFW/include"
IncludeDir["GLAD"] = "Crane/vendor/GLAD/include"
IncludeDir["ImGui"] = "Crane/vendor/imgui"
IncludeDir["glm"] = "Crane/vendor/glm"

include "Crane/vendor/GLFW"
include "Crane/vendor/GLAD"
include "Crane/vendor/imgui"

project "Crane"
	location "Crane"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crpch.h"
	pchsource "Crane/src/crpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links 
	{ 
		"GLFW",
		"GLAD",
		"ImGui",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CR_PLATFORM_WINDOWS",
			"CR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines
        {
            "CR_DEBUG",
            "CR_ENABLE_ASSERTS"
        }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CR_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Crane/vendor/spdlog/include",
		"Crane/src",
		"Crane/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Crane"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"CR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"CR_DEBUG",
			"CR_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CR_DIST"
		runtime "Release"
		optimize "On"