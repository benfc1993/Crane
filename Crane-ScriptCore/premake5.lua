project "Crane-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("../Sparrow/Resources/Scripts")
	objdir ("../Sparrow/Resources/Scripts/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.c",
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


