#include "Timer.h"

#include "Assert.h"

namespace fry_core
{
	namespace timer
	{
		struct TimerGlobals
		{
			TimerGlobals() :
		TicksPerSecond( 0 ),
			StartTick( 0 ),
			Initialized( false )
		{
		}

		s64		TicksPerSecond;
		s64		StartTick;
		bool	Initialized;
		};

		static TimerGlobals fTimerGlobals;

		void Init()
		{
			CHECK_ALWAYS(	!fTimerGlobals.Initialized );

			fTimerGlobals.TicksPerSecond = 0;
			fTimerGlobals.StartTick = 0;
			fTimerGlobals.Initialized = true;

			QueryPerformanceFrequency( (LARGE_INTEGER*)&fTimerGlobals.TicksPerSecond );
			QueryPerformanceCounter( (LARGE_INTEGER*)&fTimerGlobals.StartTick );
		}

		void Shutdown()
		{
			CHECK_ALWAYS( fTimerGlobals.Initialized );

			fTimerGlobals.TicksPerSecond = 0;
			fTimerGlobals.StartTick = 0;
		}

		double TimeSinceStart()
		{
			CHECK_ALWAYS( fTimerGlobals.Initialized );

			s64 CurrentTime = 0;
			QueryPerformanceCounter( (LARGE_INTEGER*)&CurrentTime );

			return (double)( CurrentTime - fTimerGlobals.StartTick ) / fTimerGlobals.TicksPerSecond;
		}
	}
}