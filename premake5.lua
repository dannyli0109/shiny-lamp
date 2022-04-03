workspace "App"
    architecture "x64"
    
    configurations 
    { 
        "Debug", 
        "Release" 
    }

project "App"  
    kind "ConsoleApp"   
    language "C++"   
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}" 
    objdir "obj/%{cfg.buildcfg}" 
    debugdir "Working"
    staticruntime "off"

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
        "lib/glad/**.c",
        "lib/glm/**.*",
        "lib/imgui/**.cpp",
        "lib/Assimp/include/**.*",
        "lib/boost/**.*",
        "lib/yaml-cpp/src/**.cpp",
        "lib/yaml-cpp/src/**.h",
        "lib/yaml-cpp/include/yaml-cpp/**.h"
    } 

    defines
	{
		"GLFW_INCLUDE_NONE"
	}

    includedirs
    {
        "src",
        "lib",
        "lib/glad",
        "lib/GLFW",
        "lib/glm",
        "lib/imgui",
        "lib/stb_image",
        "lib/Assimp/include",
        "lib/Assimp/lib",
        "lib/boost/uuid",
        "lib/yaml-cpp/include"
    }

    libdirs 
    { 
        "lib/Assimp/lib", 
        "lib/GLFW"
    }

    links
	{
        "opengl32.lib",
        "glfw3_mt.lib",
        "assimp-vc143-mtd.lib"
    }
    

filter "configurations:Debug"
    defines { "DEBUG" }  
    symbols "On" 

filter "configurations:Release"  
    defines { "NDEBUG" }    
    optimize "On" 

filter "system:windows"
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
