project "Optick"
	kind "StaticLib"
	language "C"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		--"OPTICK_ENABLE_GPU",
		--"OPTICK_ENABLE_GPU_VULKAN",
		--"OPTICK_ENABLE_GPU_D3D12"
	}


	files
	{
		"src/**.cpp",
		"include/**.h"
	}

	filter "configurations:Debug"
		runtime "Debug"
		optimize "Off"
        symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
        symbols "Off"

	filter "configurations:Dist"
		runtime "Release"
		optimize "On"
        symbols "Off"