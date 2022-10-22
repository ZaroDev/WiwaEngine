project "ImGuizmo"
	kind "StaticLib"
	language "C++"
    staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"*.h",
		"*.cpp"
	}
	includedirs
	{
		"../imgui"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "Off"

    filter "configurations:Dist"
		runtime "Release"
		optimize "On"
        symbols "Off"
