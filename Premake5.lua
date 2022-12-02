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
    
    Macro ={}
    Macro["DebugMode"]="AQ_DEBUG"
    Macro["Windows"]="AQ_PLATFORM_WINDOWS"
    Macro["VS_NoWarning"]="_CRT_SECURE_NO_WARNINGS"
    Macro["Yaml_StaticLibrary"]="YAML_CPP_STATIC_DEFINE"


    IncludeDir ={}
    IncludeDir["Aquarius"] ="AquariusCore/source/"
    IncludeDir["Vendor"]    ="Vendor/"
    IncludeDir["OpenGL"]   ="Vendor/OpenGL/"
    IncludeDir["IMGUI"]     ="Vendor/IMGUI/"
    IncludeDir["IMGUI"]     ="Vendor/IMGUI/"
    IncludeDir["Entt"]       ="Vendor/entt/"
    IncludeDir["Yaml"]       ="Vendor/yaml-cpp/"
    IncludeDir["ImGuizmo"]       ="AquariusCore/Vendor/ImGuizmo/"
    IncludeDir["IconsFontAwesome"]       ="Vendor/IconsFontAwesome/"

    LibDir ={}
    LibDir["GLFW"] = "Vendor/OpenGL/GLFW/build/lib-vc2022/"
    LibDir["IMGUI"] = "Vendor/IMGUI/"
    LibDir["FreeType2"] = "Vendor/freetype/"
    LibDir["Yaml"] = "Vendor/yaml-cpp/"

     Lib ={}   
     Lib["OpenGL"] = "opengl32.lib"
     Lib["GLFW"] = "glfw3.lib"
     Lib["IMGUI"] = "IMGUI.lib"
     Lib["FreeType2"] = "freetyped.lib"
     Lib["Yaml_Debug"] = "yaml-cppd.lib"
     Lib["Yaml"] = "yaml-cpp.lib"


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
            "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.h",
            "%{prj.name}/Vendor/ImGuizmo/ImGuizmo.cpp"
        }

        includedirs
        {
            "%{prj.name}/source/",
            "%{IncludeDir.Vendor}",
            "%{IncludeDir.OpenGL}",
            "%{IncludeDir.IMGUI}",
            "%{IncludeDir.Entt}",
            "%{IncludeDir.Yaml}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.IconsFontAwesome}"
        }
        libdirs
        {
            "%{LibDir.GLFW}",
            "%{LibDir.Yaml}"
        }

        links
        {
            "%{Lib.OpenGL}",
            "%{Lib.GLFW}",
            
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
                "%{Macro.Windows}",
                "%{Macro.VS_NoWarning}",
                "%{Macro.Yaml_StaticLibrary}",
            }


        filter "configurations:Debug"
            defines "AQ_Debug"
            optimize "On"
            runtime "Debug"
            defines
            {
                "%{Macro.DebugMode}",
            }
            links
            {
                "%{Lib.Yaml_Debug}",
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
                "%{Macro.DebugMode}",
            }
            links
            {
                "%{Lib.Yaml}",
            }
            buildoptions 
            {
                "/Zi"
            }

        filter "configurations:Pure"
            defines "AQ_Pure"
            optimize "On"
            runtime "Release"
        filter "files:Vendor/OpenGL/GLAD/glad.c"
            flags{"NoPCH"}
        filter "files:AquariusCore/Vendor/ImGuizmo/**.cpp"
            flags{"NoPCH"}


    project "Lyra"
        location "Lyra"
        kind "ConsoleApp"
        language "C++"

        staticruntime "off"

        targetdir ("bin/" .. outputdir.. "/%{prj.name}")
        objdir ("bin-inter/" .. outputdir.. "/%{prj.name}")

        pchheader "LyraPCH.h"
        pchsource "%{prj.name}/source/LyraPCH.cpp"
        
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
            "%{IncludeDir.Entt}",
            "%{IncludeDir.Yaml}",
            "%{IncludeDir.ImGuizmo}",
            "%{IncludeDir.IconsFontAwesome}"
        }
        libdirs
        {
            "%{LibDir.GLFW}",
            "%{LibDir.IMGUI}",
            "%{LibDir.Yaml}"
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
                "%{Macro.Windows}",
                "%{Macro.VS_NoWarning}",
                "%{Macro.Yaml_StaticLibrary}",
            }
        


        filter "configurations:Debug"
            defines "AQ_Debug"
            optimize "On"
            runtime "Debug"

            defines
            {
                "%{Macro.DebugMode}",
            }
            links
            {
                "%{Lib.Yaml_Debug}",
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
                "%{Macro.DebugMode}",
            }
            links
            {
                "%{Lib.Yaml}",
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
        filter "configurations:Pure"
            defines "AQ_Pure"
            optimize "On"
            runtime "Release"


    project "Test"
    location "Test"
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
        "%{IncludeDir.Entt}",
        "%{IncludeDir.Yaml}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.IconsFontAwesome}"
    }
    libdirs
    {
        "%{LibDir.GLFW}",
        "%{LibDir.IMGUI}",
        "%{LibDir.Yaml}"
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
            "%{Macro.Windows}",
            "%{Macro.VS_NoWarning}",
            "%{Macro.Yaml_StaticLibrary}",
        }
    


    filter "configurations:Debug"
        defines "AQ_Debug"
        optimize "On"
        runtime "Debug"

        defines
        {
            "%{Macro.DebugMode}",
        }
        links
        {
            "%{Lib.Yaml_Debug}",
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
            "%{Macro.DebugMode}",
        }
        links
        {
            "%{Lib.Yaml}",
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
                "%{Macro.DebugMode}",
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
                "%{Macro.DebugMode}",
            }
        filter "configurations:Pure"
            defines "AQ_Pure"
            optimize "On"
            runtime "Release"