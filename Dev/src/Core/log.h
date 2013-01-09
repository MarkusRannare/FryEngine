#ifndef FRYCORE_LOG_H
#define FRYCORE_LOG_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

// for va_list
#include <cstdarg>

#include "log_types.h"

#if FRY_SHIPPING
	#define debugf __noop
#else
	#define debugf log::Logf
#endif

// @TODO: Move these to a better place
// @TODO: Make platform independent logs and move these to a implementation dependent location
#define LOG_FOREGROUND_BLUE			0x0001
#define LOG_FOREGROUND_GREEN		0x0002
#define LOG_FOREGROUND_RED			0x0004
#define LOG_FOREGROUND_INTENSITY	0x0008
#define LOG_FOREGROUND_GREY			LOG_FOREGROUND_BLUE | LOG_FOREGROUND_GREEN | LOG_FOREGROUND_RED
#define LOG_FOREGROUND_YELLOW		LOG_FOREGROUND_GREEN | LOG_FOREGROUND_RED

namespace fry_core
{
	/**
	 * Ugly workaround to prevent incorrect conversion to va_list when
	 * buffered output is trying to call print its output
	 */
	struct va_wrapper
	{
		va_list Args;
	};

	namespace log
	{
		/**
		 * Call this to initialize the log object
		 * Initializes the global symbol Log* GLog, so if you
		 * want to extend the log, then you can use the it to access
		 * all the output streams
		 */
		void Init();

		/**
		 * Call this when shutting down to release all resources, also
		 * invalidates the GLog symbol
		 */
		void Shutdown();

		/**
		 * Adds a output stream that we will forward the log to
		 * Gains control of the memory and will be deallocating it
		 * when the log system is shutdown
		 *
		 * \param Stream - a non NULL stream to add
		 */
		void AddOutputStream( OutputStream* Stream );

		/**
		 * Set this to true if all comming output should be buffered and stored in main memory
		 * when it's set to false, all buffers will be flushed to all attached output streams
		 */
		void SetOutputBuffered( bool ShouldBuffer );

		/**
		 * Formats string for sending to registered output streams
		 *
		 * \param Format - standard printf format
		 * \param ... - arguments to printf
		 */
		void __cdecl Logf( const char* Format, ... );

		/**
		 * Formats string for sending to registered output streams
		 *
		 * \param Format - standard printf format
		 * \param ... - arguments to printf
		 */
		void __cdecl Logf( ELogChannel channel, const char* Format, ... );

		/**
		 * Internal function, the actual function that handles the
		 * printing to all registered output streams
		 */
		void __cdecl _Printf( ELogChannel channel, const char* Format, ... );

		/**
		 * Internal function, the actual function that handles the
		 * printing to all registered output streams
		 */
		void __cdecl _Printf( ELogChannel channel, const char* Format, va_wrapper Args );
	}
}

#endif