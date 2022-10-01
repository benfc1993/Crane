require "vscode"

workspace "Crane"
	architecture "x64"
	startproject "Sparrow"

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
IncludeDir["stb_image"] = "Crane/vendor/stb_image"
IncludeDir["entt"] = "Crane/vendor/entt/include"

group "Dependencies"
	include "Crane/vendor/GLFW"
	include "Crane/vendor/GLAD"
	include "Crane/vendor/imgui"

group ""

project "Crane"
	location "Crane"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crpch.h"
	pchsource "Crane/src/crpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/entt/include/**.hpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
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
		"_CRT_SECURE_NO_WARNINGS"
	}

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
            "CR_ENABLE_ASSERTS"
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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"Crane/vendor/spdlog/include",
		"Crane/src",
		"Crane/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}
	
	links
	{
		"Crane"
	}
	
	prebuildcommands
	{
		("{COPY} assets ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "system:linux"
		systemversion "latest"
		pic "On"
		staticruntime "off"

	
	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
	
	defines
	{
			"CR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"CR_DEBUG",
			"CR_ENABLE_ASSERTS",
			"CR_PROFILE"
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

project "Sparrow"
	location "Sparrow"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"Crane/vendor/spdlog/include",
		"Crane/src",
		"Crane/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}
	
	links
	{
		"Crane"
	}
	
	prebuildcommands
	{
		("{COPY} assets ../bin/" .. outputdir .. "/Sparrow")
	}

	filter "system:linux"
		systemversion "latest"
		pic "On"
		staticruntime "off"

	
	filter "system:windows"
		systemversion "latest"
		staticruntime "on"
	
		defines
		{
			"CR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"CR_DEBUG",
			"CR_ENABLE_ASSERTS",
			"CR_PROFILE"
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