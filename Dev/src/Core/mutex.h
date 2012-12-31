#ifndef FRYCORE_MUTEX_H
#define FRYCORE_MUTEX_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

#include "Threading_Types.h"

namespace fry_core
{
	namespace mutex
	{
		void Init( Mutex& Mutex );
		void Destroy( Mutex& Mutex );

		void Aquire( Mutex& Mutex );
		void Release( Mutex& Mutex );
	}
};

#endif // #ifndef FRYCORE_MUTEX_H