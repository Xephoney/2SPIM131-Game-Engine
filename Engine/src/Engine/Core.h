#pragma once

#ifdef ENG_PLATFORM_WINDOWS
#endif

#define BIT(x) (1 << x)

#ifdef ENG_DEBUG
	#define ENG_ENABLE_ASSERTS
#endif

#ifdef ENG_ENABLE_ASSERTS
	#define ENGINE_ASSERT(x, ...) {if(!(x)) {LOG_ERROR("Assertion Failed :{0}", __VA_ARGS__); __debugbreak();}}
	#define ENGINE_CORE_ASSERT(x, ...) {if(!(x)) {ENGINE_LOG_ERROR("Assertion Failed :{0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ENGINE_ASSERT(x, ...) 
	#define ENGINE_CORE_ASSERT(x, ...) 
#endif


#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
