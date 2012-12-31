#ifndef FRYENGINE_WINDOW_H
#define FRYENGINE_WINDOW_H

namespace fry_engine
{
	/**
	 * Window is responsible for beeing the main screen into the game, on windows
	 * this is the main window
	 */
	class Window
	{
		public:
			virtual ~Window();

			/**
			* Handles the messagepump from the OS (resize of window, o.s.o)
			*/
			virtual bool HandleMessages() = 0;

			/**
			 * \param Width - The width of the window
			 * \param Height - The height of the window
			 * \param X - x-location of window, if -1, let OS decide
			 * \param Y - y-location of window, if -1, let OS decide
			 * \return false if something went wrong during initialization of the
			 *		viewport
			 */
			virtual bool Init( int Width, int Height, int X = -1, int Y = -1 ) = 0;

			/**
			 * Actually brings forth the Window to become active
			 * \param Fullscreen - True if we want to startup the game in fullscreen
			 *		if supported
			 */
			virtual void Show( bool Fullscreen ) = 0;

			/**
			 * \return A platform specific handle for the viewport, NULL if no there
			 *		applicatable handle
			 */
			virtual void* GetHandle() const = 0;
	};

	/**
	 * Implemented on each platform to return the default game window for it
	 */
	Window* CreateGameWindow();
}

#endif