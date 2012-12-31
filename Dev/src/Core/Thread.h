#ifndef FRYCORE_THREAD_H
#define FRYCORE_THREAD_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

#include "Threading_Types.h"

namespace fry_core
{
	namespace thread
	{
		enum EThreadCreationFlags
		{
			TCF_CREATE_RUNNING = 0x0,
			TCF_CREATE_SUSPENDED = 0x00000004
		};

		typedef unsigned long (THREAD_FUNC *ThreadFunc)( void* );

		void Create( ThreadFunc ThreadFunction, const char* Name, ThreadInfo& Info, void* ThreadParam, EThreadCreationFlags CreationFlags );
		void Destory( ThreadInfo& Info );
		void Resume( ThreadInfo& Info );
	}
}

#endif