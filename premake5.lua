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
IncludeDir["GLAD"] = "Engine/3rdParty/GLAD/include"
IncludeDir["imgui"] = "Engine/3rdParty/imgui"
IncludeDir["glm"] = "Engine/3rdParty/glm"
IncludeDir["entt"] = "Engine/3rdParty/entt/include"
IncludeDir["stb_image"] = "Engine/3rdParty/stb_image/include"
IncludeDir["FMOD"] = "Engine/3rdParty/FMOD/inc"
IncludeDir["assimp"] = "Engine/3rdParty/assimp/include"


LibraryDir = {}
LibraryDir["FMOD"] = "Engine/3rdParty/FMOD/lib"
LibraryDir["assimp"] = "Engine/3rdParty/assimp/lib"

include "Engine/3rdParty/GLFW"
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

	pchheader "engpch.h"
	pchsource "Engine/src/Engine/engpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/3rdParty/glm/glm/**.hpp",
		"%{prj.name}/3rdParty/glm/glm/**.inl"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.FMOD}",
		"%{IncludeDir.entt}"
	}
	libdirs
	{
		"%{LibraryDir.FMOD}",
		"%{LibraryDir.assimp}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"imgui",
		"assimp-vc143-mt",
		"opengl32"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS",
			"GLFW_INCLUCE_NONE"
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENG_RELEASE"
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

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.FMOD}",
		"Engine/src"
	}
	libdirs
	{
		"%{LibraryDir.FMOD}"
	}
	links
	{
		"fmod_vc.lib",
		"Engine"
	}
	postbuildcommands
	{
		("{COPYFILE} %{wks.location}Engine/3rdParty/FMOD/lib/fmod.dll $(TargetDir)")
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		runtime "Release"
		optimize "on"



		