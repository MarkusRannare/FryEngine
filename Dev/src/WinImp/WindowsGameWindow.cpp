#include "WindowsGameWindow.h"

#include <Core/Assert.h>

#include <Core/System.h>

#include <Core/message_box.h>

using namespace fry_core;
using namespace fry_engine;
using namespace fry_win_imp;

extern HINSTANCE g_ExeInstance;
extern int g_ShowCmd;

static bool fIsInitialized = false;
static const char fWindowClassName[] = "FryGameWindow";

static LRESULT CALLBACK GameWindowProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

WindowsGameWindow::WindowsGameWindow() :
	mWindowHandle( NULL )
{
}

WindowsGameWindow::~WindowsGameWindow()
{
}

bool WindowsGameWindow::HandleMessages()
{
	MSG Msg;

	bool ContinueRunning = true;

	while( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) > 0 )
	{
		if( Msg.message == WM_QUIT )
		{
			ContinueRunning = false;
		}
		// @todo: Re-add TranslateMessage if we need it
		//TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}

	return ContinueRunning;
}

void WindowsGameWindow::StaticInitialize()
{
	// Register the Window class for the game viewport only once!
	WNDCLASSEX WC;
	ZeroMemory( &WC, sizeof( WNDCLASSEX ) );

	WC.cbSize = sizeof( WNDCLASSEX );
	WC.style = 0;
	WC.lpfnWndProc = GameWindowProc;
	WC.cbClsExtra = 0;
	WC.cbWndExtra = 0;
	WC.hInstance = g_ExeInstance;
	WC.hIcon = LoadIcon( NULL, IDI_APPLICATION );	// @todo: Add custom icon support
	WC.hCursor = LoadCursor( NULL, IDC_ARROW );		// @todo: Add custom cursor support
	WC.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	WC.lpszMenuName = NULL;
	WC.lpszClassName = fWindowClassName;
	WC.hIconSm = LoadIcon( NULL, IDI_APPLICATION );	// @todo: Add custom small icon support 

	if( !RegisterClassEx( &WC ) )
	{
		message_box::Show( "Window registration failed", "Error", message_box::I_Error | message_box::B_Ok );
		CHECK_ALWAYS( FALSE );
	}
}

// @todo: Add fullscreen option to this (we need to recreate a window if we are switching to fullscreen mode)
bool WindowsGameWindow::Init( int Width, int Height, int X /* = -1 */, int Y /* = -1 */ )
{
	if( !fIsInitialized )
	{
		fIsInitialized = true;
		StaticInitialize();
	}

	// If X or Y is -1, let the OS decide on the location of window
	if( X == -1 ) 
	{
		X = CW_USEDEFAULT;
	}
	if( Y == -1 ) 
	{
		Y = CW_USEDEFAULT;
	}

	// Add borders to window size so that our playheight becomes the actual window size
	Width += GetSystemMetrics(SM_CXFIXEDFRAME) * 2;										// @todo: Change this to screensize if fullscreen
	Height += GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);	// @todo: Change this to screensize if fullscreen


	// Create window from window-class
	mWindowHandle = CreateWindowEx(
		0,
		fWindowClassName,
		system::GetGameName(),	// @todo: Make this customaizeable
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, // @todo: Change this to WS_POPUP|WS_VISIBLE if we are running in fullscreen
		X, Y,
		Width, Height,
		NULL,					// Owning window
		NULL,					// Menu
		g_ExeInstance,
		NULL );
	CHECK_ALWAYS( mWindowHandle != NULL && "Failed to create window" );

	return false;
}

// @todo: Move Fullscreen to other function
void WindowsGameWindow::Show( bool Fullscreen )
{
	bool _f = Fullscreen;
	UNUSED( _f );
	
	ShowWindow( mWindowHandle, g_ShowCmd );
	UpdateWindow( mWindowHandle );
}

void* WindowsGameWindow::GetHandle() const
{
	return mWindowHandle;
}

LRESULT CALLBACK GameWindowProc( HWND Hwnd, UINT Msg, WPARAM WParam, LPARAM LParam )
{
	UNUSED( Hwnd );
	UNUSED( Msg );
	UNUSED( WParam );
	UNUSED( LParam );

	// @todo: Handle more cases here
	switch( Msg )
	{
		case WM_CLOSE:
		{
			DestroyWindow( Hwnd );
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			break;
		}
		default:
		{
			return DefWindowProc( Hwnd, Msg, WParam, LParam );
		}
	}

	return 0;
}