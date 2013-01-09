#ifndef FRYCORE_DEBUG_H
#define FRYCORE_DEBUG_H

#ifdef WIN32
	#pragma once
#endif // #ifdef WIN32

#include "Build.h"

#if WINDOWS
	#define TOGGLE_BREAKPOINT() \
		__asm \
		{ \
			int 3 \
		}
#else
	#error TOGGLE_BREAKPOINT isn't defined for the current platform
#endif // #ifdef WINDOWS

#endif
