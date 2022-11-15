project "Wiwa-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	clr "Unsafe"

	targetdir ("%{wks.location}/Editor/resources/scripts")
	objdir ("%{wks.location}/Editor/resources/scripts/intermediates")

	files 
	{
		"Source/**.cs",
		"Properties/**.cs"
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