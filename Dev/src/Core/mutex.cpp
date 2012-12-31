#include "Mutex.h"


namespace fry_core
{
	namespace mutex
	{
		void Init( Mutex& Mutex )
		{
			InitializeCriticalSection( &Mutex );
		}

		void Destroy( Mutex& Mutex )
		{
			DeleteCriticalSection( &Mutex );
		}

		void Aquire( Mutex& Mutex )
		{
			EnterCriticalSection( &Mutex );
		}

		void Release( Mutex& Mutex )
		{
			LeaveCriticalSection( &Mutex );
		}
	}
}