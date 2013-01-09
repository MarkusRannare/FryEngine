#ifndef FRYCORE_LOGTYPES_H
#define FRYCORE_LOGTYPES_H

namespace fry_core
{
	class OutputStream;

	enum ELogChannel
	{
		#define LOG_CHANNEL( CHANNEL, FORMATTING ) LG_##CHANNEL
		#include "log_channel.h"
		#undef LOG_CHANNEL
	};
}

#endif