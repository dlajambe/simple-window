workspace "simple-window"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "simple-window"
	location "simple-window"
	kind "StaticLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "10.0.22000.0"

	filter "configurations:Debug"
		defines "SW_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "SW_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "SW_DIST"
		symbols "on"

project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"simple-window/src"
	}

	links
	{
		"simple-window"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "on"
		systemversion "10.0.22000.0"

	filter "configurations:Debug"
		defines "SAND_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "SAND_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "SAND_DIST"
		symbols "on"