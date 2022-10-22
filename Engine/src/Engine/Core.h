#pragma once

#ifdef ENG_PLATFORM_WINDOWS
	#ifdef ENG_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
	#error Engine is Windows only

#endif

#define BIT(x) (1 << x)



#ifdef ENG_ENABLE_ASSERTS
	#define ENGINE_ASSERT(x, ...) {if(!(x)) {LOG_ERROR("Assertion Failed :{0}", __VA_ARGS__); __debugbreak();}}
	#define ENGINE_CORE_ASSERT(x, ...) {if(!(x)) {ENGINE_LOG_ERROR("Assertion Failed :{0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ENGINE_ASSERT(x, ...) 
	#define ENGINE_CORE_ASSERT(x, ...) 
#endif


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
