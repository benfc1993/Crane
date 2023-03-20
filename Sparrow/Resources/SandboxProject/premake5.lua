workspace "Sandbox"
	architecture "x86_64"
	startproject "SandboxProject"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SandboxProject"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Bin")
	objdir ("Intermediates")

	files
	{
		"Assets/Scripts/Source/**.cs",
		"Assets/Scripts/Properties/**.c",
    }

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
