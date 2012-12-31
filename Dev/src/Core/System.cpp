#include "System.h"
#include <cstring>
#include "Assert.h"
#include "memory.h"

static char* fCmd = NULL;
static int fInitializeCount = 0;

using namespace foundation;

namespace fry_core
{
	namespace system
	{
		void Init( const char* Cmd )
		{
			++fInitializeCount;
			CHECK_ALWAYS( fInitializeCount == 1 && "You shouldn't call Sys_Initialize more than once for each application" );

			int CmdLength = strlen( Cmd );
			fCmd = MAKE_NEW_ARR( memory_globals::default_allocator(), char, CmdLength + 1 );

			memcpy( fCmd, Cmd, CmdLength );
			fCmd[CmdLength] = '\0';
		}

		void Shutdown()
		{
			--fInitializeCount;
			CHECK_ALWAYS( fInitializeCount == 0 && "You should call Sys_Finalize as many times as you call Sys_Initialize" );
			MAKE_DELETE_ARR( memory_globals::default_allocator(), char, fCmd );
		}

		const char* GetCmdLine()
		{
			return fCmd;
		}
	}
}