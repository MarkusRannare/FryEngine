#ifndef FRYENGINE_FILEWRITER_H
#define FRYENGINE_FILEWRITER_H

#include <Core/Log.h>
#include <Core/OutputStream.h>
#include <stdio.h>

namespace fry_engine
{
	class FileWriter : public fry_core::OutputStream
	{
		public:
			virtual ~FileWriter();

			virtual void Serialize( fry_core::ELogChannel Channel, const char* String );
		protected:
			FILE* mFile;
		private:
			friend class Filesystem;

			FileWriter( FILE* File );

			// Not supported
			FileWriter();
			FileWriter( const FileWriter& Copy );
			FileWriter& operator=( const FileWriter Copy );
	};
}

#endif