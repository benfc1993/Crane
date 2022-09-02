require "vscode"


workspace "Crane"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }


    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}

    IncludeDir["GLFW"] = "Crane/vendor/GLFW/include"

    include "Crane/vendor/GLFW"

project "Crane"
    location "Crane"
    kind "SharedLib"
    language "c++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "crpch.h"
    pchsource "Crane/src/crpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
    }

    
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS",
            "CR_BUILD_DLL"
        }
        
        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "CR_DEBUG"
        symbols "on"
    
    filter "configurations:Release"
        defines "CR_RELEASE"
        optimize "on"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "c++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Crane/src",
        "Crane/vendor",
        "Crane/vendor/spdlog/include"
    }

    links
    {
        "Crane"
    }

    
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        
        defines
        {
            "CR_PLATFORM_WINDOWS",
        }
        

    filter "configurations:Debug"
        defines "CR_DEBUG"
        symbols "on"
    
    filter "configurations:Release"
        defines "CR_RELEASE"
        optimize "on"
    
    filter "configurations:Dist"
        defines "CR_DIST"
        optimize "on"