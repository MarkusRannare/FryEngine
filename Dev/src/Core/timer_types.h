#ifndef FRYCORE_TIMERTYPES_H
#define FRYCORE_TIMERTYPES_H

#include "Types.h"

namespace fry_core
{
	struct Timer
	{
		s64 _StartTime;
		s64 _PausedTicks;
		s64 _TotalPausedTicks;
	};
}

#endif