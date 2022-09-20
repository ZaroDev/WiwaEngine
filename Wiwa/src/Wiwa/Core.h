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
	#define WI_ASSERT(x, ...) {if(!(x)) { WI_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreack(); } }
	#define WI_CORE_ASSERT(x, ...) {if(!(x)) { WI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreack(); } }
#else
	#define WI_ASSERT(x, ...) 
	#define WI_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define WI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)