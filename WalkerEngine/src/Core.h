#pragma once

#ifdef W_PLATFORM_WINDOWS
	#ifdef W_BUILD_DLL
		#define WALKER_API __declspec(dllexport)
	#else
		#define WALKER_API __declspec(dllimport)
	#endif
#else
#error Only Windows is supported
#endif

#define BIT(x) (1 << x)