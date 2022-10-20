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


/* TODO (LOGGING) : Add Assertions https://youtu.be/88dmtleVywk?t=1317
#ifdef ENG_ENABLE_ASSERTS
#define ENGINE_ASSERT(x, ...) {if(!(x)) {ENGINE_ERROR("Assertion Failed :{0}")}}
#endif
*/
