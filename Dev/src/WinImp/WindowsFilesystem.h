#ifndef FRYWINIMP_WINDOWSFILESYSTEM_H
#define FRYWINIMP_WINDOWSFILESYSTEM_H

#include <Engine/Filesystem.h>

namespace fry_win_imp
{
	class WindowsFilesystem : public fry_engine::Filesystem
	{
		public:
			virtual ~WindowsFilesystem();

			/**
			 * Starts async loading of contents of file
			 *
			 * \param Filename - The filename to of the file we want to read
			 * \param Offset - Offset from start that we want to start reading from the file
			 * \param BytesToRead - How many bytes should we read from the file
			 * \param DestBuffer - A buffer that will be filled with the contents from the file
			 */
			virtual void AsyncLoad( const char* Filename, int Offset, int BytesToRead, void* DestBuffer );

			/**
			 * Gets the size of a file
			 *
			 * \param Filename the file we want the size of
			 * \return the size of the file, -1 if the file doesn't exist
			 */
			virtual int GetFileSize( const char* Filename );

			/**
			 * Iterates through a directory and fetches all files of that directory
			 * 
			 * \param Directory The directory
			 */
			virtual bool GetDirectoryContent( const char* Directory, DirectoryContent& Content, u32 Filters = LDP_All );

			/**
			 * Sets the current working directory
			 *
			 * @param Directory - Either a absolute or relative path to our working directory
			 */
			virtual void SetWorkingDirectory( const char* Directory );

			/**
			 * Gets the full path to our executable
			 *
			 * @param out_ExecutableFile - the full path to our executable
			 * @param StringLength - How big buffer have we allocated to out_ExecutableFile
			 * @return true if the buffer was successfully fitted in out_ExecutableFile
			 */
			virtual bool GetExecutableFile( char* out_ExecutableFile, int StringLength );
	};
}

#endif