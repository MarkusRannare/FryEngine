#ifndef FRYENGINE_FILESYSTEM_H
#define FRYENGINE_FILESYSTEM_H

#include <Core/collection_types.h>
#include <Core/memory_types.h>

namespace fry_core
{
	class OutputStream;
}

namespace fry_engine
{
	class Filesystem
	{
		public:
			/**
			 * What files do we want when we do a directory listing
			 */
			enum EListDirectoryIncludes
			{
				LDP_Files = 1,
				LDP_Directories = 2,
										// = 4

				// Update if the content updates here
				LDP_All = LDP_Files | LDP_Directories // Is this standard compliant? o.O
			};

			/**
			 * The results when listing all files in a directory
			 */
			struct DirectoryContent
			{
				// To determine what allocator to use
				enum EContentSize
				{
					LR_SMALL,
					LR_HUGE
				};

				/// \param ContentSize - Determines what allocator to use when we are allocating the array
				DirectoryContent( EContentSize ContentSize = LR_HUGE );

				~DirectoryContent();
				
				const char* operator[]( int Idx ) const;

				/// How many results do we have
				size_t Size() const;

				/// Internal function, used to fill the listing result
				void _AddDirectoryContent( const char* FileName );

				/// @TODO: Add another allocator that is used for the array, TempAllocator could be used here
				foundation::Allocator&			_Allocator;
				foundation::Array<const char*>	_Results;

				private:
					// No copy or assignment
					DirectoryContent( const DirectoryContent& Copy );
					DirectoryContent operator=( const DirectoryContent& From );

					foundation::Allocator& CSToAlloc( EContentSize ContentSize );
			};
		public:
			virtual ~Filesystem();

			/**
			 * Starts async loading of contents of file
			 *
			 * \param Filename - The filename to of the file we want to read
			 * \param Offset - Offset from start that we want to start reading from the file
			 * \param BytesToRead - How many bytes should we read from the file
			 * \param DestBuffer - A buffer that will be filled with the contents from the file
			 */
			virtual void AsyncLoad( const char* Filename, int Offset, int BytesToRead, void* DestBuffer ) = 0;

			/**
			 * Gets the size of a file
			 *
			 * \param Filename the file we want the size of
			 * \return the size of the file, -1 if the file doesn't exist
			 */
			virtual int GetFileSize( const char* Filename ) = 0;

			/**
			 * Iterates through a directory and fetches all files of that directory
			 * 
			 * \param Directory The directory
			 */
			virtual bool GetDirectoryContent( const char* Directory, DirectoryContent& Content, u32 Filters = LDP_All ) = 0;

			/**
			 * Sets the current working directory
			 *
			 * @param Directory - Either a absolute or relative path to our working directory
			 */
			virtual void SetWorkingDirectory( const char* Directory ) = 0;

			/**
			 * Gets the full path to our executable
			 *
			 * @param out_ExecutableFile - the full path to our executable
			 * @param StringLength - How big buffer have we allocated to out_ExecutableFile
			 * @return true if the buffer was successfully fitted in out_ExecutableFile
			 */
			virtual bool GetExecutableFile( char* out_ExecutableFile, int StringLength ) = 0;

			/**
			 * Creates a output stream at a given location
			 * @param OutputLocation - the file we want to write to with the output stream
			 * @param return NULL if we can't access the file
			 */
			fry_core::OutputStream* CreateFileWriter( const char* OutputLocation );

			/**
			 * Gets the current working directory
			 *
			 * @param out_CurrentDirectory - our current working directory
			 * @param StringLength - How big buffer have we allocated to out_CurrentDirectory
			 * @return true if the buffer was successfully fitted in out_CurrentDirectory
			 */
			//virtual bool GetCurrentDirectory( const char* out_CurrentDirectory, int StringLength ) = 0;
	};

	Filesystem* CreateFilesystem();
}

#endif