workspace "Aquarius"
    architecture "x64"
    configurations
    {
        "Debug",
        "Release",
        "Pure"
    }
    defines
        {
            "AQ_IMGUI",
            "AQ_GLM"
        }
outputdir = "%{cfg.buildcfg}_%{cfg.system}_%{cfg.architecture}"

project "AquariusCore"
    location "AquariusCore"
    kind "StaticLib"
    language "C++"

    staticruntime "off"
 

    targetdir ("bin/" .. outputdir.. "/%{prj.name}")
    objdir ("bin-inter/" .. outputdir.. "/%{prj.name}")


    pchheader "AQPCH.h"
    pchsource "%{prj.name}/source/AQPCH.cpp"
    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.cpp",
        "Vendor/OpenGL/GLAD/glad.c",
        -- "Vendor/stb_image/stb_image.cpp"
    }

    includedirs
    {
        "%{prj.name}/Vendor/",
        "%{prj.name}/source/",
        "Vendor/",
        "Vendor/OpenGL/",
        "%{prj.name}/Vendor/IMGUI/"
    }
    libdirs
    {
        "Vendor/OpenGL/GLFW/build/lib-vc2022",
        "Vendor/freetype"
    }

    links
    {
        "opengl32.lib",
        "glfw3.lib",
        "freetyped.lib"
    }
 
    -- postbuildcommands
    -- {
    --   ("{copy} ../bin/"..outputdir.."/AquariusCore/AquariusCore.dll ../bin/"..outputdir.."/Aquarius") 
    -- }
    
    filter "system:windows"
        cppdialect "C++20"
        systemversion "10.0.19041.0"


        defines
        {
            "AQ_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }


    filter "configurations:Debug"
        defines "AQ_Debug"
        optimize "On"
        runtime "Debug"
        defines
        {
            "AQ_DEBUG"
        }

        buildoptions 
        {
            "/Zi"
        }
    filter "configurations:Release"
        defines "AQ_Release"
        optimize "On"
        runtime "Release"

        defines
        {
            "AQ_DEBUG"
        }
    filter "configurations:Pure"
        defines "AQ_Pure"
        optimize "On"
        runtime "Release"
    filter "files:Vendor/OpenGL/GLAD/glad.c"
        flags{"NoPCH"}

project "Aquarius"
    location "Aquarius"
    kind "ConsoleApp"
    language "C++"

    staticruntime "off"

    targetdir ("bin/" .. outputdir.. "/%{prj.name}")
    objdir ("bin-inter/" .. outputdir.. "/%{prj.name}")

    files
    {
        "%{prj.name}/source/**.h",
        "%{prj.name}/source/**.cpp"
    }

    includedirs
    {
        "AquariusCore/source/",
        "%{prj.name}/Vendor/",
        "%{prj.name}/source/",
        "AquariusCore/Vendor/",
        "AquariusCore/Vendor/IMGUI",
        "Vendor/OpenGL/",
        "Vendor/"
    }
    libdirs
    {
        "Vendor/OpenGL/GLFW/build/lib-vc2022",
        "Vendor/freetype",
        "AquariusCore/Vendor/IMGUI"
    }
    
    links
    {
        "AquariusCore",
        "opengl32.lib",
        "glfw3.lib",
        "freetyped.lib",
        "IMGUI.lib"
    }
    flags
    {
   
    }



    filter "system:windows"
        cppdialect "C++20"
        systemversion "10.0.19041.0"

        defines
        {
            "AQ_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
    


    filter "configurations:Debug"
        defines "AQ_Debug"
        optimize "On"
        runtime "Debug"

        defines
        {
            "AQ_DEBUG"
        }
        buildoptions 
        {
            "/Zi"
        }
        linkoptions 
        {
            "/NODEFAULTLIB:libcmtd.lib" ,
            "/LTCG"
        }

    filter "configurations:Release"
        defines "AQ_Release"
        optimize "On"
        runtime "Release"

        defines
        {
            "AQ_DEBUG"
        }
    filter "configurations:Pure"
        defines "AQ_Pure"
        optimize "On"
        runtime "Release"