/**
 * Some things here should be moven into a common launch project, as here we should just catch up the windows launch and do some
 * windows specific things before entering the "actual" main loop
 */

#include <Core/Types.h>

extern INT FryMain( const char* CommandLine );

HINSTANCE g_ExeInstance; // @todo: Maybe change to use GetModuleHandle() all the time?
int g_ShowCmd;

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	g_ExeInstance = hInstance;
	g_ShowCmd = nShowCmd;

	UNUSED( hPrevInstance );
	UNUSED( nShowCmd );
	
	INT ReturnValue = FryMain( lpCmdLine );

	return ReturnValue;
}