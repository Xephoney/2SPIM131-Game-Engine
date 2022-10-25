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
	kind "SharedLib"
	language"C++"
	staticruntime "Off"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("int/" .. outputdir .. "/%{prj.name}")

	pchheader "Engine/engpch.h"
	pchsource "Engine/src/Engine/engpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
		--"glad.c"
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
		cppdialect "C++17"
		staticruntime "On"
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
		staticruntime "off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		staticruntime "off"
		runtime "Release"
		optimize "On"

	filter "configurations:Distribution"
		defines "ENG_DIST"
		staticruntime "off"
		runtime "Release"
		symbols "Off"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp"
	}

	includedirs
	{
		"Engine/3rdParty/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"Engine/3rdParty",
		"Engine/src"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		optimize "On"

	filter "configurations:Distribution"
		defines "ENG_DIST"
		symbols "Off"