#include "WindowsFilesystem.h"

#include <Winbase.h>

#include <Core/array.h>

#include <Core/Types.h>
#include <Core/Log.h>

using namespace fry_core;
using namespace foundation;
using namespace fry_engine;
using namespace fry_win_imp;

WindowsFilesystem::~WindowsFilesystem()
{
}

void WindowsFilesystem::AsyncLoad( const char* Filename, int Offset, int BytesToRead, void* DestBuffer )
{
	char* Tmp = (char*)Filename;
	UNUSED( Tmp );
	UNUSED( Offset );
	UNUSED( BytesToRead );
	UNUSED( DestBuffer );
}

bool WindowsFilesystem::GetDirectoryContent( const char* Directory, DirectoryContent& Content, u32 Filters /* = LDP_ALL*/ )
{
	WIN32_FIND_DATA FFD;
	ZeroMemory( &FFD, sizeof( WIN32_FIND_DATA ) );

	HANDLE hFind = FindFirstFile( Directory, &FFD );

	if( hFind == INVALID_HANDLE_VALUE )
	{
		DWORD Error = GetLastError();
		debugf( LG_Filesystem, "Couldn't list directory: %x", Error );
		return false;
	}

	do
	{
		if( ( FFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && ( Filters & LDP_Directories ) )
		{
			Content._AddDirectoryContent( FFD.cFileName );
		}
		else if( !( FFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) && ( Filters & LDP_Files ) )
		{
			Content._AddDirectoryContent( FFD.cFileName );
		}
	} while( FindNextFile( hFind, &FFD ) != 0 );

	FindClose( hFind );

	return true;
}

int WindowsFilesystem::GetFileSize( const char* Filename )
{
	char* Temp = (char*)Filename;

	UNUSED( Temp );

	return -1;
}

void WindowsFilesystem::SetWorkingDirectory( const char* Directory )
{
	SetCurrentDirectory( Directory );
}

bool WindowsFilesystem::GetExecutableFile( char* out_ExecutableFile, int StringLength )
{
	return GetModuleFileName( NULL, out_ExecutableFile, StringLength ) == ERROR_SUCCESS;
}