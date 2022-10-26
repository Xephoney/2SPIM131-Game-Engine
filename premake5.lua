workspace "Game Engine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories relative to root folder 
IncludeDir = {}
IncludeDir["spdlog"] = "Engine/3rdParty/spdlog/include"
IncludeDir["GLFW"] = "Engine/3rdParty/GLFW/include"
IncludeDir["imgui"] = "Engine/3rdParty/imgui"
IncludeDir["GLAD"] = "Engine/3rdParty/GLAD/include"
IncludeDir["glm"] = "Engine/3rdParty/glm"


include "Engine/3rdParty/GLAD"
include "Engine/3rdParty/imgui"

project "Engine"
	location "Engine"
	kind "StaticLib"
	language"C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("int/" .. outputdir .. "/%{prj.name}")

	pchheader "Engine/engpch.h"
	pchsource "Engine/src/Engine/engpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}"
	}

	libdirs
	{
		"%{prj.name}/3rdParty/GLFW/lib"
	}

	links
	{
		"glfw3",
		"GLAD",
		"imgui",
		"opengl32"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS",
			"ENG_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "ENG_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	libdirs
	{
		"Engine/3rdParty/GLFW/lib"
	}

	includedirs
	{
		"Engine/3rdParty/spdlog/include",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"Engine/3rdParty",
		"Engine/src"
	}

	links
	{
		"Engine",
		"opengl32",
		"glfw3"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		optimize "on"

	filter "configurations:Distribution"
		defines "ENG_DIST"
		optimize "on"
		symbols "off"



		