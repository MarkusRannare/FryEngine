#ifndef FRYCORE_OUTPUTSTREAM_H
#define FRYCORE_OUTPUTSTREAM_H

#include "log_types.h"

namespace fry_core
{
	class OutputStream
	{
		public:
			virtual ~OutputStream();

			virtual void Serialize( ELogChannel Channel, const char* String ) = 0;
		private:
	};
}

#endif