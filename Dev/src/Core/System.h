#ifndef FRYCORE_SYSTEM_H
#define FRYCORE_SYSTEM_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

namespace fry_core
{
	namespace system
	{
		/// Initializes the system and stores the provided Cmd
		void Init( const char* Cmd );
		/// Shutdowns and cleanups the system
		void Shutdown();

		/// return a complete game name that can be used (MyGame - Game of the year)
		const char* GetGameName();
		
		/// return a short game name, for the game MyGame - Game of the year, this is My
		const char* GetShortGameName();

		/// Gets the string from the CMD line
		const char* GetCmdLine();
	}
}

#endif