workspace "Crane"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Crane"
    location "Crane"
    kind "SharedLib"
    language "c++"

    targetdir ("bin/" .. outputir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {

    }

    
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        
        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
        }
        
        -- postbuildcommands
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputir .. "/Sandbox")
        -- }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "on"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "on"
    
    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "c++"

    targetdir ("bin/" .. outputir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs 
    {
        "Crane/src"
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
            "HZ_PLATFORM_WINDOWS",
        }
        

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        symbols "on"
    
    filter "configurations:Release"
        defines "HZ_RELEASE"
        optimize "on"
    
    filter "configurations:Dist"
        defines "HZ_DIST"
        optimize "on"