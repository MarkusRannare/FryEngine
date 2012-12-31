#include "HighPerfTimer.h"

#include "memory.h"

using namespace fry_core;
using namespace foundation;

HighPerfTimer::HighPerfTimer()
{
	m_pTicksPerSecond.QuadPart = 0;
	m_pStartTicks.QuadPart = 0;
	m_pPausedTicks.QuadPart = 0;
	m_pTotalPausedTicks.QuadPart = 0;
 
	// Get the high resolution counter's frequency
	QueryPerformanceFrequency( &m_pTicksPerSecond );
}

HighPerfTimer::~HighPerfTimer()
{
}

void HighPerfTimer::Start(void)
{
	// Reset the Paused ticks count.
	m_pPausedTicks.QuadPart = 0;
	m_pTotalPausedTicks.QuadPart = 0;
 
	// Get the starting ticks.
	QueryPerformanceCounter( &m_pStartTicks );
}

void HighPerfTimer::Pause(void)
{
	// Start the paused ticks count.
	QueryPerformanceCounter( &m_pPausedTicks );
}

void HighPerfTimer::Resume(void)
{
	  if( m_pStartTicks.QuadPart != 0 )
	  {
			// End the paused ticks count.
			LARGE_INTEGER endPausedTicks;
			QueryPerformanceCounter(&endPausedTicks);
			if (endPausedTicks.QuadPart > m_pPausedTicks.QuadPart)
				  m_pTotalPausedTicks.QuadPart += endPausedTicks.QuadPart - m_pPausedTicks.QuadPart;
			m_pPausedTicks.QuadPart = 0;
	  }
	  else
	  {
			Start();
	  }
}

void HighPerfTimer::GetTime(s32& hours, s32& minutes, double& seconds)
{
	  LARGE_INTEGER ticks;
	  double time;
 
	  // Get the latest tick count.
	  QueryPerformanceCounter(&ticks);
 
	  // If the timer is paused, discount the time since it was paused.
	  LARGE_INTEGER immediatePausedTicks = m_pTotalPausedTicks;
	  if (m_pPausedTicks.QuadPart > 0)
			immediatePausedTicks.QuadPart += ticks.QuadPart - m_pPausedTicks.QuadPart;
 
	  // Convert the elapsed ticks into number of seconds
	  time = (double)(ticks.QuadPart - immediatePausedTicks.QuadPart - m_pStartTicks.QuadPart)/(double)m_pTicksPerSecond.QuadPart;
 
	  // Number of hours
	  hours = (s32)time/3600;
 
	  // Number of minutes
	  time = time - (hours * 3600);
	  minutes = (s32)time/60;
 
	  // Number of seconds
	  seconds = time - (minutes * 60);
}