#ifndef FRYCORE_THREADING_TYPES_H
#define FRYCORE_THREADING_TYPES_H

#include "Build.h"
#include "Types.h"

namespace fry_core
{
	struct ThreadInfo
	{
		char*			Name;
		int				ThreadHandle;
		unsigned long	ThreadId;
	};

#if WINDOWS
	typedef HANDLE Semaphore;
	typedef CRITICAL_SECTION Mutex;
#else
	#error Threading types haven't been defined for the current platform
#endif
}

#endif