#pragma once

#ifdef WI_PLATFORM_WINDOWS
	#ifdef WI_BUILD_DLL 
		#define WI_API __declspec(dllexport)
	#else
		#define WI_API __declspec(dllimport)
	#endif
#else
	#error WIWA ONLY SUPPORTS WINDOWS!
#endif

#ifdef WI_ENABLE_ASSERTS
	#define WI_ASSERT(x, ...) {if(!(x)) { WI_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } }
	#define WI_ASSERT_MSG(...) { WI_ERROR("Assertion Failed: {0}", __VA_ARGS__);  __debugbreak(); } 
	#define WI_CORE_ASSERT(x, ...) {if(!(x)) { WI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define WI_CORE_ASSERT_MSG(...) { WI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}
#else
	#define WI_ASSERT(x, ...) 
	#define WI_ASSERT_MSG(...)
	#define WI_CORE_ASSERT(x, ...)
	#define WI_CORE_ASSERT_MSG(...)
#endif


#define BIT(x) (1 << (x))
#define CLAMP(x, mn, mx) (x) < (mn) ? (x) = (mn) : (x) = (x); (x) > (mx) ? (x) = (mx) : (x) = (x)

#define PI 3.14159265359
#define PI_F 3.14159265359f

#define WI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)