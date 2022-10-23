project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	
	includedirs
	{
		"src",
		"%{wks.location}/Crane/vendor/spdlog/include",
		"%{wks.location}/Crane/src",
		"%{wks.location}/Crane/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}
	
	links
	{
		"Crane",
		"vulkan",
		"shaderc_shared",
		"spirv-cross-c",
		"spirv-cross-core",
		"spirv-cross-cpp",
		"spirv-cross-glsl",
		"spirv-cross-reflect",
		"spirv-cross-util",
		"spirv-cross-c-shared",
		"spirv-cross-hlsl",
		"spirv-cross-msl",
		"z",
	}
	
	prebuildcommands
	{
		("{COPY} assets %{wks.location}/bin/" .. outputdir .. "/Sandbox")
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
