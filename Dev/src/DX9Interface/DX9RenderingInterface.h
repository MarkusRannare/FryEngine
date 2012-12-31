#ifndef FRYENGINE_DX9_H
#define FRYENGINE_DX9_H

#include <Engine/RenderingInterface.h>

#include <d3dx9shader.h>
#include <d3d9.h>

namespace DX9Interface
{
	class DX9RenderingInterface : public fry_engine::RenderingInterface
	{
		public:
			DX9RenderingInterface();

			virtual ~DX9RenderingInterface();

			/**
			 * \return the rendering implementation we have
			 */
			virtual fry_engine::RenderingInterface::ERenderinInterfaceType GetRIT() const;

			/**
			 * Initializes the RenderingInterface
			 * \param WindowHandle The window we want to render to
			 */
			virtual void Init( void* WindowHandle );

			/**
			 * Called when tearing down the entire rendering and
			 * cleanups all resources
			 */
			virtual void Shutdown();

			/**
			 * Clears the current scene
			 * @todo: Add parameters
			 */
			virtual void Clear();

			/**
			 * Swaps the old framebuffer to the new
			 * @todo: Add parameters
			 */
			virtual void Present();

			/**
			 * Draws a primitive from user memory, so no need to create any vertex buffer as this
			 *	is transfered each frame (good for dynamic objects that change their vertices all the time)
			 *
			 *	@note This uses a triangle list beneath the hood, no other primitives supported (yet)
			 *	@note use SetVertexDeclaration before calling this to setup how the stream is setup
			 *	@note only supports one vertex stream
			 *
			 * @param PrimitiveCount The number of triangles in to draw
			 * @param VertexStream A pointer to CPU memory to be transfered and drawn
			 * @param VertexSize How big is our Vertex structure (in bits)
			 */
			virtual void DrawPrimitiveUp( unsigned int PrimitiveCount, void* VertexStream, unsigned int VertexSize );
		private:
			// Unimplemented operators that shouldn't be used
			DX9RenderingInterface( const DX9RenderingInterface& Other );
			DX9RenderingInterface& operator=( const DX9RenderingInterface& Other );
		public:

			LPDIRECT3D9			mD3DInterface;
			LPDIRECT3DDEVICE9	mD3DDevice;
	};
}

#endif