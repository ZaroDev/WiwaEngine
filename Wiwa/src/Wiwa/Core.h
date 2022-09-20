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

#ifdef ZO_ENABLE_ASSERTS
	#define ZO_ASSERT(x, ...) {if(!(x)) { ZO_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreack(); } }
	#define ZO_CORE_ASSERT(x, ...) {if(!(x)) { ZO_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreack(); } }
#else
	#define ZO_ASSERT(x, ...) 
	#define WI_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)

#define ZO_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)