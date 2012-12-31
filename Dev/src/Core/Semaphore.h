#ifndef FRYCORE_SEMAPHORE_H
#define FRYCORE_SEMAPHORE_H

#ifdef WIN32
	#pragma once
#endif // #ifdef WIN32

#include "Threading_Types.h"

namespace fry_core
{
	namespace semaphore
	{
		void Init( Semaphore& Semaphore, int InitialCount = 1, int MaximumCount = 1, const char* Name = NULL );
		void Destroy( Semaphore& Semaphore );
		void Wait( Semaphore& Semaphore );
		void Signal( Semaphore& Semaphore );
	}
}

#endif