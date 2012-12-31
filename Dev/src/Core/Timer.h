#ifndef FRYCORE_TIMER_H
#define FRYCORE_TIMER_H

#include "timer_types.h"

namespace fry_core
{
	namespace timer
	{
		void Init();
		void Shutdown();

		double TimeSinceStart();

		void Start( Timer& Timer );
		void Pause( Timer& Timer );
		void Stop( Timer& Timer );
	}
}

#endif