#include "Filesystem.h"

#include <Core/array.h>
#include <cstring>
#include <Core/Assert.h>
#include "FileWriter.h"

using namespace foundation;

namespace fry_engine
{
	Filesystem::~Filesystem()
	{
	}

	Filesystem::DirectoryContent::DirectoryContent( EContentSize ContentSize /** = LR_MEDIUM */ ) :
		_Allocator( CSToAlloc( ContentSize ) ),
		_Results( _Allocator )
	{
	}

	Filesystem::DirectoryContent::~DirectoryContent()
	{
		for( size_t Idx = 0; Idx < foundation::array::size( _Results ); ++Idx )
		{
			_Allocator.deallocate( (void*)_Results[ Idx ] );
		}
	}

	void Filesystem::DirectoryContent::_AddDirectoryContent( const char* FileName )
	{
		size_t StrLen = strlen( FileName );
		
		char* File = (char*)_Allocator.allocate( StrLen + 1 );

		strcpy_s( File, StrLen + 1, FileName );

		foundation::array::push_back( _Results, (const char*)File );
	}

	const char* Filesystem::DirectoryContent::operator[]( int Idx ) const
	{
		return _Results[ Idx ];
	}

	size_t Filesystem::DirectoryContent::Size() const
	{
		return foundation::array::size( _Results );
	}

	foundation::Allocator& Filesystem::DirectoryContent::CSToAlloc( EContentSize ContentSize )
	{
		switch( ContentSize )
		{
			case Filesystem::DirectoryContent::LR_HUGE:
			{
				return foundation::memory_globals::default_allocator();
				break;
			}
			case Filesystem::DirectoryContent::LR_SMALL:
			{
				return foundation::memory_globals::default_scratch_allocator();
				break;
			}
			default:
			{
				CHECK_ALWAYS( false && "No such EContentSize" );
				break;
			}
		}
		return foundation::memory_globals::default_allocator();
	}

	fry_core::OutputStream* Filesystem::CreateFileWriter( const char* OutputLocation )
	{
		FILE* File = fopen( OutputLocation, "w+" );

		if( File != NULL )
		{
			return MAKE_NEW( memory_globals::default_allocator(), FileWriter, File );
		}

		return NULL;
	}
}