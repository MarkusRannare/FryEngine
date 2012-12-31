#include "WinConsoleOutputStream.h"

#include <Core/Log.h>
#include <Core/Types.h>
#include <Core/Assert.h>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

extern void Cleanup();

static char ColorLookup[] =
{
#define LOG_CHANNEL( CHANNEL, FORMATTING ) FORMATTING
#include <Core/log_channel.h>
#undef LOG_CHANNEL
};

namespace fry_win_imp
{
	static BOOL WINAPI HandlerRoutine( __in DWORD dwCtrlType );

	WinConsoleOutputStream::WinConsoleOutputStream() :
		mOutputHandle( NULL )
	{
		BOOL Success = AllocConsole();

		CHECK_ALWAYS( Success && "Failed to allocate console" );
		UNUSED( Success );

		// @TODO: Cleanup the console code
		mOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );
		int CrtHandle = _open_osfhandle( (long)mOutputHandle, _O_TEXT );
		FILE* fStd_fout = _fdopen( CrtHandle, "w" );
		setvbuf( fStd_fout, NULL, _IONBF, 1 );
		*stdout = *fStd_fout;

		// Setup a message handler so we can shutdown properly if closing the console
		SetConsoleCtrlHandler( HandlerRoutine, true );
	}

	WinConsoleOutputStream::~WinConsoleOutputStream()
	{
		// Remove our message handler
		SetConsoleCtrlHandler( HandlerRoutine, false );
		FreeConsole();
	}
	
	/**
	 * Handles messages from the console, we use this to make sure we do a proper shutdown when closing the game 
	 * by pressing the X on the console 
	 **/
	BOOL WINAPI HandlerRoutine( __in DWORD dwCtrlType )
	{
		switch( dwCtrlType )
		{
			case CTRL_CLOSE_EVENT:
			{
				::Cleanup();

				return true;
			}
			default:
			{
				return false;
			}
		}
	}
	
	void WinConsoleOutputStream::Serialize( fry_core::ELogChannel Channel, const char* String )
	{
		SetConsoleTextAttribute( mOutputHandle, ColorLookup[ Channel ] );

		printf( "%s", String );
	}
}