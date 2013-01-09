#ifndef FRYENGINE_IEXEC_H
#define FRYENGINE_IEXEC_H

#ifdef WIN32
#pragma once
#endif // #ifdef WIN32

namespace fry_engine
{
	class IExec
	{
		public:
			virtual ~IExec();

			virtual bool Exec( const char* Command ) = 0;
	};
}

#endif