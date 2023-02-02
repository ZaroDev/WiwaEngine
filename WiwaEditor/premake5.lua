project "WiwaEditor"
	kind "ConsoleApp"
	language "C++"

	targetdir ("%{wks.location}/bin/".. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/".. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDirs.SPDLOG}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.GLM}",
		"%{IncludeDirs.Wiwa}",
		"%{IncludeDirs.Optick}",
		"%{IncludeDirs.ImGuizmo}",
		"%{IncludeDirs.filewatch}",
		"%{IncludeDirs.IconFontCppHeaders}"
	}

	links
	{
		"Wiwa",
		"ImGui",
		"Optick",
		"ImGuizmo"
	}

	debugdir "%{wks.location}/Editor"

	filter "system:windows"
		cppdialect "C++latest"
		staticruntime "Off"
		systemversion "latest"

		defines
		{
			"WI_PLATFORM_WINDOWS"
		}
		postbuildcommands
		{
			("{COPY} \"%{cfg.buildtarget.relpath}\" \"%{wks.location}Editor\"")
		}

	filter "configurations:Debug"
		defines "WI_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "WI_RELEASE"
		runtime "Release"

		symbols "On"
		optimize "On"

	filter "configurations:Dist"
		defines "WI_DIST"
		runtime "Release"
		symbols "Off"
		optimize "On"
