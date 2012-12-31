#ifndef FRYENGINE_RENDERINGINTERFACE_H
#define FRYENGINE_RENDERINGINTERFACE_H

#include <Core/Types.h>

namespace fry_engine
{
	class RenderingInterface
	{
		public:
			enum ERenderinInterfaceType
			{
				RIT_DX9,
				RIT_DX11,
				RIT_OpenGL,
				RIT_OpenGL2ES
			};
		public:
			virtual ~RenderingInterface();
	
			/**
			 * \return the rendering implementation we have
			 */
			virtual ERenderinInterfaceType GetRIT() const = 0;

			/**
			 * Initializes the RenderingInterface
			 * \param WindowHandle The window we want to render to
			 */
			virtual void Init( void* WindowHandle ) = 0;
			
			/**
			 * Called when tearing down the entire rendering and
			 * cleanups all resources
			 */
			virtual void Shutdown() = 0;

			/**
			 * Clears the current scene
			 * @todo: Add parameters
			 */
			virtual void Clear() = 0;

			/**
			 * Swaps the old framebuffer to the new
			 * @todo: Add parameters
			 */
			virtual void Present() = 0;

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
			virtual void DrawPrimitiveUp( unsigned int PrimitiveCount, void* VertexStream, unsigned int VertexSize ) = 0;
		private:
	};

	RenderingInterface* CreateRenderingInterface( RenderingInterface::ERenderinInterfaceType InterfaceType );
}

#endif