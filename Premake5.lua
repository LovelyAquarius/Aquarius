workspace "Aquarius"
    architecture "x64"
    startproject "Lyra"
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
    


    IncludeDir ={}
    IncludeDir["Aquarius"] ="AquariusCore/source/"
    IncludeDir["Vendor"]    ="Vendor/"
    IncludeDir["OpenGL"]   ="Vendor/OpenGL/"
    IncludeDir["IMGUI"]     ="Vendor/IMGUI/"
    IncludeDir["IMGUI"]     ="Vendor/IMGUI/"
    IncludeDir["Entt"]       ="Vendor/entt/"


    LibDir ={}
    LibDir["GLFW"] = "Vendor/OpenGL/GLFW/build/lib-vc2022/"
    LibDir["IMGUI"] = "Vendor/IMGUI/"
    LibDir["FreeType2"] = "Vendor/freetype/"


     Lib ={}   
     Lib["OpenGL"] = "opengl32.lib"
     Lib["GLFW"] = "glfw3.lib"
     Lib["IMGUI"] = "IMGUI.lib"
     Lib["FreeType2"] = "freetyped.lib"



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
        }

        includedirs
        {
            "%{prj.name}/source/",
            "%{IncludeDir.Vendor}",
            "%{IncludeDir.OpenGL}",
            "%{IncludeDir.IMGUI}",
            "%{IncludeDir.Entt}"
        }
        libdirs
        {
            "%{LibDir.GLFW}",
        }

        links
        {
            "%{Lib.OpenGL}",
            "%{Lib.GLFW}"
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


    project "Lyra"
        location "Lyra"
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
            "%{prj.name}/source/",
            "%{IncludeDir.Aquarius}",
            "%{IncludeDir.Vendor}",
            "%{IncludeDir.IMGUI}",
            "%{IncludeDir.OpenGL}",
            "%{IncludeDir.Entt}"
        }
        libdirs
        {
            "%{LibDir.GLFW}",
            "%{LibDir.IMGUI}",
        }
        
        links
        {
            "AquariusCore",
            "%{Lib.OpenGL}",
            "%{Lib.GLFW}",
            "%{Lib.IMGUI}",
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
            "%{prj.name}/source/",
            "%{IncludeDir.Aquarius}",
            "%{IncludeDir.Vendor}",
            "%{IncludeDir.IMGUI}",
            "%{IncludeDir.OpenGL}",
            "%{IncludeDir.Entt}"
        }
        libdirs
        {
            "%{LibDir.GLFW}",
            "%{LibDir.IMGUI}",
        }
        
        links
        {
            "AquariusCore",
            "%{Lib.OpenGL}",
            "%{Lib.GLFW}",
            "%{Lib.IMGUI}"
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