#ifndef WINCONSOLE_OUTPUTSTREAM_H
#define WINCONSOLE_OUTPUTSTREAM_H

#include <Core/Types.h>
#include <Core/OutputStream.h>

namespace fry_win_imp
{
	class WinConsoleOutputStream : public fry_core::OutputStream
	{
		public:
			WinConsoleOutputStream();
			virtual ~WinConsoleOutputStream();

			virtual void Serialize( fry_core::ELogChannel Channel, const char* String );
		private:
			HANDLE mOutputHandle;
	};
}

#endif