#include "Thread.h"

#include "Assert.h"
#include "memory.h"

using namespace foundation;
using namespace fry_core;

static const int MAX_THREADS = 10;
static int fNumThreads = 0;

static ThreadInfo* AllocatedThreads[MAX_THREADS];

#include "Log.h"

namespace fry_core
{
	namespace thread
	{
		void Create( ThreadFunc ThreadFunction, const char* Name, ThreadInfo& Info, void* ThreadParam, EThreadCreationFlags CreationFlags )
		{
			if( fNumThreads < MAX_THREADS )
			{
				Info.ThreadHandle = (int)CreateThread( 
					NULL,				// default security attributes
					0,					// use edfault stack size  
					ThreadFunction,		// thread function name
					ThreadParam,		// argument to thread function
					CreationFlags,		// use creation flags
					&Info.ThreadId );	// returns the thread identifier

				size_t ThreadNameLength = strlen( Name );
				Info.Name = MAKE_NEW_ARR( memory_globals::default_allocator(), char, ThreadNameLength + 1 );
				memcpy( Info.Name, Name, ThreadNameLength );
				Info.Name[ThreadNameLength] = '\0';

				AllocatedThreads[fNumThreads++] = &Info;
			}
			else
			{
				debugf( LG_Error, "Couldn't create thread %s", Name );
			}
		}

		void Destory( ThreadInfo& Info )
		{
			WaitForSingleObject( (HANDLE)Info.ThreadHandle, INFINITE );
			CloseHandle( (HANDLE)Info.ThreadHandle );
			Info.ThreadHandle = NULL;

			--fNumThreads;
			MAKE_DELETE_ARR( memory_globals::default_allocator(), char, Info.Name );
		}

		void Resume( ThreadInfo& Info )
		{
			ResumeThread( (HANDLE)Info.ThreadHandle );
		}
	}
}