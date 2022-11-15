workspace "AppAssembly"
	architecture "x86_64"
	startproject "Wiwa-AppAssembly"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}


project "Wiwa-AppAssembly"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	clr "Unsafe"

	targetdir ("%{wks.location}/resources/scripts")
	objdir ("%{wks.location}/resources/scripts/intermediates")

	files 
	{
		"%{wks.location}/Assets/**.cs"
	}
	links
	{
		"%{wks.location}/resources/scripts/Wiwa-ScriptCore.dll"
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"