#ifndef CORE_HIGHPERFTIMER_H
#define CORE_HIGHPERFTIMER_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

#include <Windows.h>
#include "Types.h"

namespace fry_core
{	
	// TODOCOMMENT: Document HighPerfTimer
	class HighPerfTimer
	{
		public:
			// TODOCOMMENT
			HighPerfTimer();
			~HighPerfTimer();
		 
			// TODOCOMMENT
			void Start(void);
			// TODOCOMMENT
			void Pause(void);
			// TODOCOMMENT
			void Resume(void);
		 
			// TODOCOMMENT
			void GetTime(s32& hours, s32& minutes, double& seconds);
		private:
			_LARGE_INTEGER m_pTicksPerSecond;      // The counter's frequency
			_LARGE_INTEGER m_pStartTicks;          // The starting point in ticks
			_LARGE_INTEGER m_pPausedTicks;         // The amount of ticks spent when the timer was last paused
			_LARGE_INTEGER m_pTotalPausedTicks;    // The amount of ticks spent paused in total
	};
}

#endif