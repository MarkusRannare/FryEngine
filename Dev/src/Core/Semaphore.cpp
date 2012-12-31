#include "Semaphore.h"

#include "Assert.h"

namespace fry_core
{
	namespace semaphore
	{
		void Init( Semaphore& Semaphore, int InitialCount /* = 1 */, int MaximumCount /* = 1 */, const char* Name /* = NULL */ )
		{
			Semaphore = CreateSemaphore( NULL,
				InitialCount,
				MaximumCount,
				Name );

			CHECK_ALWAYS( Semaphore != NULL && "Failed to create semaphore" );
		}

		void Destroy( Semaphore& Semaphore )
		{
			CloseHandle( Semaphore );
		}

		void Wait( Semaphore& Semaphore )
		{
			WaitForSingleObject( Semaphore,
				INFINITE );
		}

		void Signal( Semaphore& Semaphore )
		{
			BOOL Result = ReleaseSemaphore( Semaphore, 1, NULL );

			CHECK_ALWAYS( Result && "Failed to release semaphore" );
			UNUSED( Result );
		}
	}
}
