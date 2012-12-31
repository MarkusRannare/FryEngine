#ifndef FRYCORE_LOGTYPES_H
#define FRYCORE_LOGTYPES_H

#include "memory.h"
#include "collection_types.h"

namespace fry_core
{
	class OutputStream;

	enum ELogChannel
	{
		#define LOG_CHANNEL( CHANNEL, FORMATTING ) LG_##CHANNEL
		#include "log_channel.h"
		#undef LOG_CHANNEL
	};

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

		~Log()
		{
		}

		foundation::Array<OutputStream*> _OutputStream;
		foundation::Array< OutputBuffer > _BufferedOutput;
		bool _ShouldBuffer;
	};
}

#endif