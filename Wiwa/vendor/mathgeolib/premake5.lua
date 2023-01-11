project "MathGeoLib"
	kind "StaticLib"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"MATH_AVX"
	}
	
	files
	{
		"src/**.cpp",
		"src/**.h"
	}
	
	filter "system:windows"
		defines "WIN32"

	filter "configurations:Debug"
		runtime "Debug"
		defines "_DEBUG"
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