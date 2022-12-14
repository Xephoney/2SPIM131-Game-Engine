workspace "Game Engine"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags
	{
		"MultiProcessorCompile"
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
IncludeDir["Jolt"] = "Engine/3rdParty/jolt/include"
IncludeDir["ImGuizmo"] = "Engine/3rdParty/ImGuizmo/"
--IncludeDir["bullet3"] = "Engine/3rdParty/bullet3/include"
--IncludeDir["yaml_cpp"] = "Engine/3rdParty/yaml-cpp/include"

LibraryDir = {}
LibraryDir["FMOD"] = "Engine/3rdParty/FMOD/lib"
LibraryDir["assimp"] = "Engine/3rdParty/assimp/lib"
--LibraryDir["bullet3"] = "Engine/3rdParty/bullet3/libs"
LibraryDir["Jolt"] = "Engine/3rdParty/jolt/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Engine/3rdParty/GLFW"
	include "Engine/3rdParty/GLAD"
	include "Engine/3rdParty/imgui"
	include "Engine/3rdParty/Jolt"
--	include "Engine/3rdParty/yaml-cpp"
group ""

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
		"%{prj.name}/3rdParty/glm/glm/**.inl",

		"%{prj.name}/3rdParty/assimp/include/assimp/**.hpp",
		"%{prj.name}/3rdParty/assimp/include/assimp/**.cpp",
		"%{prj.name}/3rdParty/assimp/include/assimp/**.h",

		"%{prj.name}/3rdParty/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/3rdParty/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.assimp}",
		--"%{IncludeDir.bullet3}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Jolt}",
		"%{IncludeDir.ImGuizmo}"
		--"%{IncludeDir.yaml_cpp}"
	}
	libdirs
	{
		"%{LibraryDir.FMOD}",
		--"%{LibraryDir.bullet3}",
		"%{LibraryDir.Jolt}",
		"%{LibraryDir.assimp}",
		
	}

	links
	{
		"GLFW",
		"GLAD",
		"imgui",
		--"yaml-cpp",
		"assimp-vc143-mt",
		"opengl32",
		"Jolt"
	}

	filter "files:Engine/3rdParty/ImGuizmo/**.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS",
			"BT_USE_DOUBLE_PRECISION",
			"BT_THREADSAFE=1",
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
		"%{IncludeDir.yaml_cpp}",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.h",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.cpp"
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.FMOD}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.Jolt}",
		"Engine/3rdParty/jolt/src",
		"%{IncludeDir.ImGuizmo}",
		"Engine/src"
	}
	libdirs
	{
		"%{LibraryDir.FMOD}",
		"%{LibraryDir.Jolt}",
		--"%{LibraryDir.bullet3}",
		"%{LibraryDir.assimp}"
	}
	links
	{
		"fmod_vc.lib",
		"assimp-vc143-mt",
		"Jolt",
		"Engine"
	}
	postbuildcommands
	{
		("{COPYFILE} %{wks.location}%{LibraryDir.FMOD}/fmod.dll $(TargetDir)"),
		("{COPYFILE} %{wks.location}%{LibraryDir.assimp}/assimp-vc143-mt.dll $(TargetDir)")
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
			--"BT_USE_DOUBLE_PRECISION",
			--"BT_THREADSAFE=1",

		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		runtime "Release"
		optimize "on"



		