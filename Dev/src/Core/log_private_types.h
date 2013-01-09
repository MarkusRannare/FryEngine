/**
 * Include this if you want to extend the log with more functionality
 */
#ifndef FRYCORE_LOG_PRIVATE_TYPES_H
#define FRYCORE_LOG_PRIVATE_TYPES_H

#include "memory.h"
#include "collection_types.h"
#include "log_types.h"

namespace fry_core
{
	/**
	 * Internal representation used for the log. This should never really need to be used
	 * unless you want to extend the logs functionality
	 */
	struct Log
	{
		struct OutputBuffer
		{
			ELogChannel _LogChannel;
			char*		_String;
		};

		Log() :
			_OutputStream( foundation::memory_globals::default_allocator() ),
			_BufferedOutput( foundation::memory_globals::default_allocator() ),
			_ShouldBuffer( false )
		{
		}

		foundation::Array<OutputStream*> _OutputStream;
		foundation::Array<OutputBuffer> _BufferedOutput;
		bool _ShouldBuffer;
	};
}

#endif