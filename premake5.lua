workspace "simple-window"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "glfw"
	location "vendor/glfw"
	kind "StaticLib"
	language "C"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"vendor/%{prj.name}/include/glfw/glfw3.h",
		"vendor/%{prj.name}/include/glfw/glfw3native.h",
		"vendor/%{prj.name}/src/glfw_config.h",
		"vendor/%{prj.name}/src/init.c",
		"vendor/%{prj.name}/src/input.c",
		"vendor/%{prj.name}/src/monitor.c",
		"vendor/%{prj.name}/src/vulkan.c",
		"vendor/%{prj.name}/src/window.c"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "10.0.22000.0"

		files
		{
			"vendor/%{prj.name}/src/win32_init.c",
			"vendor/%{prj.name}/src/win32_joystick.c",
			"vendor/%{prj.name}/src/win32_monitor.c",
			"vendor/%{prj.name}/src/win32_time.c",
			"vendor/%{prj.name}/src/win32_thread.c",
			"vendor/%{prj.name}/src/win32_window.c",
			"vendor/%{prj.name}/src/wgl_context.c",
			"vendor/%{prj.name}/src/egl_context.c",
			"vendor/%{prj.name}/src/osmesa_context.c",
			"vendor/%{prj.name}/src/**.c"
		}

		defines 
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"

project "simple-window"
	location "simple-window"
	kind "StaticLib"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"vendor/glfw/include/",
		"vendor/glew/include/"
	}

	libdirs
	{
		"vendor/glew/lib/Release/x64/"
	}

	links
	{
		"glfw",
		"opengl32.lib",
		"glew32s.lib"
	}

	defines
	{
		"GLEW_STATIC"
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
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"simple-window/src",
		"vendor/glfw/include/",
		"vendor/glew/include/"
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