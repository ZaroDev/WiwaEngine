workspace "Wiwa"
	architecture "x64"
	startproject "Sandbox"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Wiwa/vendor/GLFW/include"
IncludeDirs["Glew"] = "Wiwa/vendor/Glew/include/GL"
IncludeDirs["ImGui"] = "Wiwa/vendor/imgui"
IncludeDirs["GLM"] = "Wiwa/vendor/glm/glm"
IncludeDirs["PCG"] = "Wiwa/vendor/PCG/include"
IncludeDirs["JSON"] = "Wiwa/vendor/rapidjson"
IncludeDirs["XML"] = "Wiwa/vendor/pugixml/src"
IncludeDirs["Optick"] = "Wiwa/vendor/Optick/include"
IncludeDirs["assimp"] = "Wiwa/vendor/assimp/include"
IncludeDirs["ImGuizmo"] = "Wiwa/vendor/imguizmo"
 
include "Wiwa/vendor/GLFW"
include "Wiwa/vendor/imgui"
include "Wiwa/vendor/Optick"
include "Wiwa/vendor/imguizmo"

project "Wiwa"
	location "Wiwa"
	kind "SharedLib"
	language "C++"

	
	targetdir ("bin/".. outputdir .. "/%{prj.name}")
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")

	pchheader "wipch.h"
	pchsource "Wiwa/src/wipch.cpp"

	ignoredefaultlibraries 
	{
		"libcmtd"
	}
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"Wiwa/src",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.Glew}",
		"%{IncludeDirs.PCG}",
		"%{IncludeDirs.JSON}",
		"%{IncludeDirs.XML}",
		"%{IncludeDirs.Optick}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.ImGuizmo}"
	}

	links
	{
		"GLFW",
		"ImGui",
		"opengl32.lib",
		"Wiwa/vendor/Glew/lib/glew32.lib",
		"Optick",
		"ImGuizmo"
	}

	filter "configurations:Release"

	filter "configurations:Dist"


	filter "files:%{prj.name}/vendor/imguizmo/**.cpp"
		flags {"NoPCH"}
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"WI_PLATFORM_WINDOWS",
			"WI_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}


		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"),
			("{COPY} %{cfg.buildtarget.relpath} ../Editor")
		}


	debugdir "$(SolutionDir)/Editor"
	
	filter "configurations:Debug"
		defines "WI_DEBUG"
		staticruntime "off"
		runtime "Debug"
		symbols "On"
		links
		{
			"Wiwa/vendor/assimp/lib/x64/debug/assimp-vc143-mtd.lib"
		}

	filter "configurations:Release"
		defines "WI_RELEASE"
		staticruntime "off"
		runtime "Release"
		optimize "On"
		links
		{
			"Wiwa/vendor/assimp/lib/x64/release/assimp-vc143-mt.lib"
		}
	filter "configurations:Dist"
		defines "WI_DIST"
		staticruntime "off"
		runtime "Release"
		optimize "On"
		links
		{
			"Wiwa/vendor/assimp/lib/x64/release/assimp-vc143-mt.lib"
		}

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/".. outputdir .. "/%{prj.name}")
	objdir ("bin-int/".. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Wiwa/vendor/spdlog/include",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.GLM}",
		"Wiwa/src",
		"%{IncludeDirs.Optick}",
		"%{IncludeDirs.ImGuizmo}"
	}

	links
	{
		"Wiwa",
		"ImGui",
		"Optick",
		"ImGuizmo"
	}

	debugdir "$(SolutionDir)/Editor"

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"WI_PLATFORM_WINDOWS"

		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../Editor")
		}

	filter "configurations:Debug"
		defines "WI_DEBUG"
		staticruntime "off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "WI_RELEASE"
		staticruntime "off"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "WI_DIST"
		staticruntime "off"
		runtime "Release"
		optimize "On"
