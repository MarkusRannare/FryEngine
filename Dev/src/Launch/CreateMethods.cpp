#include <DX9Interface/DX9RenderingInterface.h>
#include <WinImp/WinConsoleOutputStream.h>
#include <WinImp/WindowsGameWindow.h>
#include <WinImp/WindowsFilesystem.h>

#include <Core/Build.h>

#include <Core/Assert.h>

#include <Core/memory.h>

using namespace foundation;

namespace fry_engine
{
	RenderingInterface* CreateRenderingInterface( RenderingInterface::ERenderinInterfaceType InterfaceType )
	{
		RenderingInterface* RI = NULL;

		switch( InterfaceType )
		{
			case RenderingInterface::RIT_DX9:
			{
#if WINDOWS
				RI = MAKE_NEW( memory_globals::default_renderthread_allocator(), DX9Interface::DX9RenderingInterface );
#endif
				break;
			}
			default:
				CHECK_ALWAYS( false && "unknown rendering interface" );
				break;
		}

		CHECK_ALWAYS( RI != NULL && "Trying to create a unsupported rendering interface on the current platform" );
		return RI;
	}

	Window* CreateGameWindow()
	{
#if WINDOWS
		return MAKE_NEW( memory_globals::default_allocator(), fry_win_imp::WindowsGameWindow );
#else
		#error No window class implemented for the current platform
		return NULL;
#endif
	}

	Filesystem* CreateFilesystem()
	{
#if WINDOWS
		return MAKE_NEW( memory_globals::default_allocator(), fry_win_imp::WindowsFilesystem );
#else
		#error No filesystem class implemented for the current platform
		return NULL;
#endif
	}
}

fry_core::OutputStream* CreateConsole()
{
#if WINDOWS
	return MAKE_NEW( memory_globals::default_allocator(),  fry_win_imp::WinConsoleOutputStream );
#else
	#error No console class specified for the current platform
	return NULL;
#endif
}