#ifndef FRYWINIMP_WINDOWSGAMEVIEWPORT_H
#define FRYWINIMP_WINDOWSGAMEVIEWPORT_H

#include <Core/Types.h>
#include <Engine/Window.h>

namespace fry_win_imp
{
	/**
	 * Windows implementation of the viewport
	 */
	class WindowsGameWindow : public fry_engine::Window
	{
		public:
			WindowsGameWindow();

			virtual ~WindowsGameWindow();

			/**
			 * Handles the messagepump from Windows
			 */
			virtual bool HandleMessages();

			/**
			 * \param Width - The width of the window
			 * \param Height - The height of the window
			 * \param X - x-location of window, if -1, let OS decide
			 * \param Y - y-location of window, if -1, let OS decide
			 * \return false if something went wrong during initialization of the
			 *		viewport
			 */
			virtual bool Init( int Width, int Height, int X = -1, int Y = -1 );

			/**
			 * Actually brings forth the Window to become active
			 * \param Fullscreen - True if we want to startup the game in fullscreen
			 *		if supported
			 */
			virtual void Show( bool Fullscreen );

			/**
			 * \return The HWND handle for the viewport if it have been initialized, NULL if initialization
			 *		went wrong if not initialized
			 */
			virtual void* GetHandle() const;
		private:
			/**
			 * Run first time we are initialized, does some first time initialization
			 */
			static void StaticInitialize();
		protected:
			HWND	mWindowHandle;
	};
}

#endif