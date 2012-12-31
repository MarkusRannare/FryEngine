#ifndef FRYCORE_SYSTEM_H
#define FRYCORE_SYSTEM_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

namespace fry_core
{
	namespace system
	{
		void Init( const char* Cmd );
		void Shutdown();

		const char* GetGameName();
		const char* GetShortGameName();

		/**
		 * Gets the string from the CMD line
		 */
		const char* GetCmdLine();
	}
}

#endif