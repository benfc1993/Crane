require "vscode"
include "Dependencies.lua"

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


group "Dependencies"
	include "Crane/vendor/Box2D"
	include "Crane/vendor/GLFW"
	include "Crane/vendor/GLAD"
	include "Crane/vendor/imgui"
	include "Crane/vendor/yaml-cpp"

group ""

include "Crane"
include "Sandbox"
include "Sparrow"
