#include <Core/Build.h>
#include <Core/Timer.h>
#include <Core/Types.h>

#include <Core/memory.h>

#include <Core/System.h>
#include <Core/string_stream.h>
#include <Core/temp_allocator.h>
#include <Core/Assert.h>
#include <Core/Log.h>

#include <Engine/Filesystem.h>
fry_engine::Filesystem* g_Filesystem = NULL;

bool g_HaveRunShutdownRutine = false;

void Cleanup();

#include <Core/Thread.h>
#include <Core/Semaphore.h>

#include <Core/message_box.h>
#include <Engine/RenderingInterface.h>

using namespace fry_engine;

RenderingInterface* g_RI;

#include <Engine/Window.h>
Window* g_Window = NULL;

#include <Core/Vector2.h>

struct RenderingThreadParams
{
	void* WindowHandle;
};

unsigned long THREAD_FUNC RenderingThreadFunc( void* StartupParameters );

#include <iostream>
#include <Core/array.h>

using namespace foundation;
using namespace fry_core;

OutputStream* CreateConsole();

Semaphore MainThreadFinishedWorking;
Semaphore RenderThreadFinishedWorking;
ThreadInfo RenderThread;


/**
 * Main loop for engine, used over different platforms
 */
int FryMain( const char* CommandLine )
{
	timer::Init();
	foundation::memory_globals::init();

	system::Init( CommandLine );

	semaphore::Init( MainThreadFinishedWorking, 0, 1, "MainThreadSempahore" );
	semaphore::Init( RenderThreadFinishedWorking, 1, 1, "RenderThreadSemaphore" );
	
	log::Init();
	// If -log
	log::AddOutputStream( CreateConsole() );
	log::SetOutputBuffered( true );
	
	g_Filesystem = CreateFilesystem();

	//char ExecutableFile[260];
	//g_Filesystem->GetExecutableFile( ExecutableFile, 260 );

	// Scope here so the LogFile buffer gets deallocated timely
	{
		string_stream::Buffer LogFile( memory_globals::default_allocator() );
		string_stream::printf( LogFile, "../%sGame/Log/Launch.log", system::GetShortGameName() );
		log::AddOutputStream( g_Filesystem->CreateFileWriter( string_stream::c_str( LogFile ) ) );
	}

	log::SetOutputBuffered( false );

	// Make sure we don't allocate unnecessary stack space
	{
		char ExecutableFile[512];
		g_Filesystem->GetExecutableFile( ExecutableFile, 512 ); 

		char* LastPathSeparator = strrchr( ExecutableFile, PATH_SEPARATOR );
		char ExecutableDirectory[512];

		int DirectoryLength = (int)LastPathSeparator - (int)ExecutableFile + 1;
		strncpy_s( ExecutableDirectory, 512, ExecutableFile, DirectoryLength );
		ExecutableDirectory[ DirectoryLength ] = '\0';

		g_Filesystem->SetWorkingDirectory( ExecutableDirectory );
	}	

	g_Window = CreateGameWindow();

	// @TODO: Fetch the correct value for this from ini's and Commandline
	bool Fullscreen = false;

	g_Window->Init( 800, 600 );

	RenderingThreadParams RenderParams;
	RenderParams.WindowHandle = g_Window->GetHandle();
	ZeroMemory( &RenderThread, sizeof( ThreadInfo ) );
	thread::Create( RenderingThreadFunc, "Rendering Thread", RenderThread, &RenderParams, thread::TCF_CREATE_RUNNING );

	g_Window->Show( Fullscreen );

	// @todo: Add support for disable rendering when game is in background / minimized
	// @todo: Add support for pausing game when in background / minimized
	while( g_Window->HandleMessages() )
	{
		// Tick

		Sleep( 4 );

		// Render
		semaphore::Wait( RenderThreadFinishedWorking );
		//log::Printf( "Pushing data to rendering thread" );
		//Sleep( 1 );
		semaphore::Signal( MainThreadFinishedWorking );
	}

	// Note, add all cleanup code in Cleanup, if not added there, it
	// won't be called if you close by clicking the console window
	Cleanup();

	return 0;
}

/** Called from the Console when exiting too */
void Cleanup()
{
	if( !g_HaveRunShutdownRutine )
	{
		// Wait for the render thread to stop working so we can make sure we have run the shutdown rutine
		semaphore::Wait( RenderThreadFinishedWorking );
		g_HaveRunShutdownRutine = true;
		// Now the renderthread will check the g_HaveRunShutdownRutine flag and gracefully shutdown
		semaphore::Signal( MainThreadFinishedWorking );

		// Waits for renderthread to be run the last frame and then destroys the thread
		thread::Destory( RenderThread );

		MAKE_DELETE( memory_globals::default_allocator(), Window, g_Window );
		MAKE_DELETE( memory_globals::default_allocator(), Filesystem, g_Filesystem );

		log::Shutdown();

		system::Shutdown();

		foundation::memory_globals::shutdown();
		timer::Shutdown();
	}
}

#include <DX9Interface/DX9RenderingInterface.h>


using namespace DX9Interface;

struct VertexStruct
{
	float X, Y, Z, W;
	DWORD Color;
	//byte R, G, B, A;
};

char* ReadContentsOfShaderFile( const char* FileName );

unsigned long THREAD_FUNC RenderingThreadFunc( void* StartupParameters )
{
	// @todo: Choose rendering interface depending on platform
	// @todo: Make it available to change interface depending on startup parameter
	// @todo: Make it available to change interface depending on ini-config
	g_RI = CreateRenderingInterface( RenderingInterface::RIT_DX9 );

	RenderingThreadParams* Params = (RenderingThreadParams*)StartupParameters;

	g_RI->Init( Params->WindowHandle );

	LPDIRECT3DDEVICE9 D3DDevice = ((DX9RenderingInterface*)(g_RI))->mD3DDevice;

	// @TODO: This should be stored with the content, as it's the content that decides what kind of data we want to
	// have whenever it is rendered
	// @TODO: Encapsulate this in RI
	// Setup vertex declaration
	D3DVERTEXELEMENT9 TestDecl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		D3DDECL_END()
	};

	LPDIRECT3DVERTEXDECLARATION9 VertexDecl;

	// @TODO: Encapuslate this in RI
	D3DDevice->CreateVertexDeclaration( TestDecl, &VertexDecl );

	// @TODO: Fix OS-independant file-readers that do this
	char* Verteshader = ReadContentsOfShaderFile( "../VillageGame/Shaders/Vertexshader.vsh" );
	char* Pixelshader = ReadContentsOfShaderFile( "../VillageGame/Shaders/Pixelshader.psh" );

	LPD3DXBUFFER CompiledVertexShader;
	LPD3DXBUFFER VertexShaderErrors;

	LPD3DXBUFFER CompiledPixelShader;
	LPD3DXBUFFER PixelShaderErrors;

	// @TODO: Encapsulate this (RI? Or RIHelper?)
	// @TODO: This can be stored precompiled on disc so that we don't have to compile the shaders at each startup
	HRESULT Result = D3DXCompileShader( Verteshader, 
		strlen( Verteshader ),
		NULL,					// Defines
		NULL,					// Includes
		"main",					// Entrypoint
		"vs_2_0",				// Shader profile
		0,						// Flags
		&CompiledVertexShader,
		&VertexShaderErrors,
		NULL );

	if( Result != D3D_OK )
	{
		debugf( LG_ShaderCompiling, "Error compiling vertex shader: %s", VertexShaderErrors->GetBufferPointer() );
	}

	if( VertexShaderErrors != NULL )
	{
		VertexShaderErrors->Release();
	}

	IDirect3DVertexShader9* VertexShader;
	Result = D3DDevice->CreateVertexShader( (DWORD*)CompiledVertexShader->GetBufferPointer(), &VertexShader );

	if( Result != D3D_OK )
	{
		debugf( LG_Error, "Error creating vertex shader" );
	}

	if( CompiledVertexShader != NULL )
	{
		CompiledVertexShader->Release();
	}

	// @TODO: Encapsulate this (RI? Or RIHelper?)
	// @TODO: This can be stored precompiled on disc so that we don't have to compile the shaders at each startup
	Result = D3DXCompileShader( Pixelshader, 
		strlen( Pixelshader ),
		NULL,					// Defines
		NULL,					// Includes
		"main",					// Entrypoint
		"ps_2_0",				// Shader profile
		0,						// Flags
		&CompiledPixelShader,
		&PixelShaderErrors,
		NULL );

	if( Result != D3D_OK )
	{
		debugf( LG_Error, "Error compiling pixel shader: %s", PixelShaderErrors->GetBufferPointer() );
	}

	IDirect3DPixelShader9* PixelShader;
	Result = D3DDevice->CreatePixelShader( (DWORD*)CompiledPixelShader->GetBufferPointer(), &PixelShader );

	if( Result != D3D_OK )
	{
		debugf( LG_Error, "Error creating pixel shader" );
	}

	if( CompiledPixelShader != NULL )
	{
		CompiledPixelShader->Release();
	}


	if( PixelShaderErrors != NULL )
	{
		PixelShaderErrors->Release();
	}

	memory_globals::default_renderthread_allocator().deallocate( Verteshader );
	memory_globals::default_renderthread_allocator().deallocate( Pixelshader );
	
	// Wait for the main thread to have done it's work before the first frame
	semaphore::Wait( MainThreadFinishedWorking );

	/**
	 * A quad, wind it counter clockwise
	 * -----
	 * |\ 2|
	 * | \ |
	 * | 1\|
	 * ----
	 */
	VertexStruct Vertices[] =
	{
		// First triangle - top left
		{ -0.5f, -0.5f, 0, 1, D3DCOLOR_XRGB( 255, 0, 0 ) },
		// First triangle - bottom left
		{ -0.5f, 0.5f, 0, 1, D3DCOLOR_XRGB( 0, 255, 0 ) },
		// First triangle - bottom right
		{ 0.5f, 0.5f, 0, 1, D3DCOLOR_XRGB( 0, 0, 255 ) },
		// Second triangle - top left
		{ -0.5f, -0.5f, 0, 1, D3DCOLOR_XRGB( 255, 0, 0 ) },
		// Second triangle - bottom right
		{ 0.5f, 0.5f, 0, 1, D3DCOLOR_XRGB( 0, 0, 255 ) },
		// Second triangle - top right
		{ 0.5f, -0.5f, 0, 1, D3DCOLOR_XRGB( 0, 255, 0 ) }
	};

	while( !g_HaveRunShutdownRutine )
	{
		// @TODO: Implement command-que (ring-buffer?) with commands from the main thread to synchronize data between the threads


		// We signal after we have fetched the commands from the main thread so that the main thread can go on and process the next frame
		semaphore::Signal( RenderThreadFinishedWorking );

		g_RI->Clear();

		D3DDevice->BeginScene();

		HRESULT Result = D3DDevice->SetVertexDeclaration( VertexDecl );
		CHECK_ALWAYS( Result == D3D_OK );
		Result = D3DDevice->SetVertexShader( VertexShader );
		CHECK_ALWAYS( Result == D3D_OK );
		Result = D3DDevice->SetPixelShader( PixelShader );
		CHECK_ALWAYS( Result == D3D_OK );
		g_RI->DrawPrimitiveUp( 2, Vertices, sizeof( VertexStruct ) );

		D3DDevice->EndScene();

		g_RI->Present();
		
		// Before check if we have shutdown, wait for the main thread
		semaphore::Wait( MainThreadFinishedWorking );
	}

	// @TODO: Teardown function
	if( VertexDecl != NULL )
	{
		VertexDecl->Release();
	}

	if( VertexShader != NULL )
	{
		VertexShader->Release();
	}
	if( PixelShader != NULL )
	{
		PixelShader->Release();
	}

	g_RI->Shutdown();
	MAKE_DELETE( memory_globals::default_renderthread_allocator(), RenderingInterface, g_RI );

	return 0;
}

#include <cstdio>

/**
 * \note Need to appFree the return value if it's != NULL
 */
char* ReadContentsOfShaderFile( const char* FileName )
{
	char* Buffer = 0;
	long Length;
	FILE* File = fopen( FileName, "rb" );

	if( File )
	{
		fseek( File, 0, SEEK_END );
		Length = ftell( File );
		fseek( File, 0, SEEK_SET );
		Buffer = (char*)memory_globals::default_renderthread_allocator().allocate( Length + 1 );
		if( Buffer )
		{
			fread( Buffer, 1, Length, File );
			// Make sure the string is zero-delimited, otherwise we are going
			// to have a bad time =)
			Buffer[Length] = '\0';
		}
		fclose( File );
	}

	return Buffer;
}
