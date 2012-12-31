#include "DX9RenderingInterface.h"
#include <Core/Types.h>
#include <Core/Log.h>

#include <Core/Assert.h>

using namespace fry_core;
using namespace fry_engine;
using namespace DX9Interface;

static bool fInitialized = false;

DX9RenderingInterface::DX9RenderingInterface() :
	mD3DDevice( NULL ),
	mD3DInterface( NULL )
{
}

DX9RenderingInterface::~DX9RenderingInterface()
{
	CHECK_ALWAYS( !fInitialized && "Destroying rendering interface before it's deinitialized" );
}

RenderingInterface::ERenderinInterfaceType DX9RenderingInterface::GetRIT() const
{
	return RenderingInterface::RIT_DX9;
}

void DX9RenderingInterface::Init( void* WindowHandle )
{
	HWND WindowHandleImp = (HWND)WindowHandle;

	mD3DInterface = Direct3DCreate9( D3D_SDK_VERSION );

	D3DPRESENT_PARAMETERS PresentParameters;
	ZeroMemory( &PresentParameters, sizeof(D3DPRESENT_PARAMETERS) );
	PresentParameters.Windowed = TRUE;						// @todo: Allow fullscreen
	PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresentParameters.hDeviceWindow = WindowHandleImp;

	mD3DInterface->CreateDevice( D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		WindowHandleImp,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&PresentParameters,
		&mD3DDevice );

	CHECK_ALWAYS( mD3DDevice != NULL && "Failed to create device" );

	fInitialized = true;
}

void DX9RenderingInterface::Shutdown()
{
	if( mD3DDevice != NULL )
	{
		mD3DDevice->Release();
	}

	if( mD3DInterface != NULL )
	{
		mD3DInterface->Release();
	}

	fInitialized = false;
}

void DX9RenderingInterface::Clear()
{
	mD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 40, 100 ), 1.0f, 0 );
}

void DX9RenderingInterface::Present()
{
	mD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void DX9RenderingInterface::DrawPrimitiveUp( unsigned int PrimitiveCount, void* VertexStream, unsigned int VertexSize )
{
	CHECK_ALWAYS( VertexSize != 0 && "Vertex size can't be zero when calling DrawPrimitiveUp" );

	HRESULT Result = mD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, PrimitiveCount, VertexStream, VertexSize );

	CHECK_ALWAYS( Result == D3D_OK && "Something went wrong during DrawPrimitiveUp" );	
	UNUSED( Result );
}