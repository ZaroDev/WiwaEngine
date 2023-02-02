project "Wiwa"
	kind "SharedLib"
	language "C++"

	
	targetdir ("%{wks.location}/bin/".. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/".. outputdir .. "/%{prj.name}")

	pchheader "wipch.h"
	pchsource "src/wipch.cpp"

	ignoredefaultlibraries 
	{
		"libcmtd"
	}
	
	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDirs.SPDLOG}",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.filewatch}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.Glew}",
		"%{IncludeDirs.PCG}",
		"%{IncludeDirs.JSON}",
		"%{IncludeDirs.mono}",
		"%{IncludeDirs.XML}",
		"%{IncludeDirs.Optick}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.ImGuizmo}",
		"%{IncludeDirs.Wwise}",
		"%{IncludeDirs.IconFontCppHeaders}"
	}

	links
	{
		"Wiwa-ScriptCore",
		"GLFW",
		"ImGui",
		"opengl32.lib",
		"%{wks.location}/Wiwa/vendor/Glew/lib/glew32.lib",
		"Optick",
		"ImGuizmo",
		"%{Library.mono}",
		"%{wks.location}/Wiwa/vendor/assimp/lib/x64/release/assimp-vc143-mt.lib",
		"%{Library.Wwise_SE}",
		"%{Library.Wwise_MMgr}",
		"%{Library.Wwise_SMgr}",
		"%{Library.Wwise_ME}",
		"%{Library.Wwise_SA}"
	}

	filter "files:vendor/imguizmo/**.cpp"
		flags {"NoPCH"}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
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
			("{COPY} \"%{cfg.buildtarget.relpath}\" \"%{wks.location}Editor\"")
		}
		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.Version}",
			"%{Library.BCrypt}"
		}

	debugdir "%{wks.location}/Editor"
	
	filter "configurations:Debug"
		defines
		{ 
			"WI_DEBUG",
			"WI_ENABLE_ASSERTS"
		}	
		runtime "Debug"
		symbols "On"


	filter "configurations:Release"
		defines "WI_RELEASE"
		runtime "Release"
		optimize "On"
		symbols "On"

	filter "configurations:Dist"
		defines "WI_DIST"
		runtime "Release"
		optimize "On"
		symbols "Off"