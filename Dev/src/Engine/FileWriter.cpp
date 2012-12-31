#include "FileWriter.h"

#include <Core/Assert.h>

using namespace foundation;

namespace fry_engine
{
	FileWriter::FileWriter( FILE* File ) :
		mFile( File )
	{
		CHECK_ALWAYS( File != NULL );
	}

	FileWriter::~FileWriter()
	{
		// @TODO: Refactor, we open the file in Filesystem, then we should close it there too!
		fclose( mFile );
	}

	void FileWriter::Serialize( fry_core::ELogChannel Channel, const char* String )
	{
		UNUSED( Channel );

		fprintf( mFile, "%s", String );
	}
}