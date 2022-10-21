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
--IncludeDir = {}
--IncludeDir["GLFW"] = "Engine/3rdParty/include/"


project "Engine"
	location "Engine"
	kind "SharedLib"
	language"C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.hpp",
		"glad.c"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/3rdParty/spdlog/include",
		"Libraries/lib",
		"Libraries/include"
	}

	libdirs
	{
		"Libraries/lib"
	}

	links
	{
		"glfw3",
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
			"_WINDLL"
		}
		
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Distribution"
		defines "ENG_DIST"
		buildoptions "/MD"
		symbols "Off"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"

	language "C++"
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
		"Libraries/include",
		"Libraries/lib",
		"Engine/src"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
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