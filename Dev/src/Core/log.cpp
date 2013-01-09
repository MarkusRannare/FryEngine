#include "Log.h"
#include "log_private_types.h"

#include "System.h"
#include "string_stream.h"
#include "temp_allocator.h"

#include "Types.h"
#include "array.h"
#include "OutputStream.h"
#include "memory.h"
#include "Assert.h"

#include "Timer.h"

using namespace fry_core;
using namespace foundation;

fry_core::Log* GLog = NULL;

static const int MAX_PRINTF = 1024;

namespace fry_core
{
	namespace log
	{
		static char* ChannelNames[] =
		{
			#define LOG_CHANNEL( CHANNEL, FORMATTING ) #CHANNEL
			#include "log_channel.h"
			#undef LOG_CHANNEL
		};


		void Init()
		{
			CHECK_ALWAYS( GLog == NULL && "Log already initialized" );

			GLog = MAKE_NEW( memory_globals::default_allocator(), Log );
		}

		void Shutdown()
		{
			CHECK_ALWAYS( GLog != NULL && "Log isn't initialized" );

			for( size_t Idx = 0; Idx < array::size( GLog->_OutputStream ); ++Idx )
			{
				MAKE_DELETE( memory_globals::default_allocator(), OutputStream, GLog->_OutputStream[ Idx ] );
			}

			MAKE_DELETE( memory_globals::default_allocator(), Log, GLog );
		}

		void AddOutputStream( OutputStream* Stream )
		{
			array::push_back( GLog->_OutputStream, Stream );
		}

		void Logf( const char* Format, ... )
		{
			using namespace string_stream;
			TempAllocator1024 Ta;

			Buffer FormatBuffer( Ta );
			FormatBuffer << Format << "\n";

			_Printf( LG_Debug, "[%.2f] %s: ", timer::TimeSinceStart(), ChannelNames[ LG_Debug ] );

			va_list VaryingParameters;
			va_start( VaryingParameters, Format );

			_Printf( LG_Debug, c_str( FormatBuffer ), VaryingParameters );

			va_end( VaryingParameters );
		}

		void Logf( ELogChannel Channel, const char* Format, ... )
		{
			using namespace string_stream;
			TempAllocator1024 Ta;

			Buffer FormatBuffer( Ta );
			FormatBuffer << Format << "\n";
			
			_Printf( Channel, "[%.2f] %s: ", timer::TimeSinceStart(), ChannelNames[ Channel ] );

			va_list VaryingParameters;
			va_start( VaryingParameters, Format );

			_Printf( Channel, c_str( FormatBuffer ), VaryingParameters );

			va_end( VaryingParameters );
		}

		void _Printf( ELogChannel Channel, const char* Format, ... )
		{
			va_list VaryingParameters;
			va_start( VaryingParameters, Format );

			va_wrapper Wrapper = { VaryingParameters };
			_Printf( Channel, Format, Wrapper );

			va_end( VaryingParameters );
		}

		void _Printf( ELogChannel Channel, const char* Format, va_wrapper Args )
		{
			using namespace string_stream;

			static char LogMessage[MAX_PRINTF];

			char* BufferToUse = !GLog->_ShouldBuffer ? LogMessage : (char*)memory_globals::default_allocator().allocate( MAX_PRINTF );

			_vsnprintf_s( BufferToUse, MAX_PRINTF, MAX_PRINTF, Format, Args.Args );

			if( !GLog->_ShouldBuffer )
			{
				for( size_t Idx = 0; Idx < array::size( GLog->_OutputStream ); ++Idx )
				{
					OutputStream* Stream = GLog->_OutputStream[ Idx ];

					Stream->Serialize( Channel, LogMessage );
				}
			}
			else
			{
				Log::OutputBuffer Buffer = { Channel, BufferToUse };
				array::push_back( GLog->_BufferedOutput, Buffer );
			}
		}

		void SetOutputBuffered( bool ShouldBuffer )
		{
			GLog->_ShouldBuffer = ShouldBuffer;

			if( !ShouldBuffer )
			{
				for( size_t Idx = 0; Idx < array::size( GLog->_BufferedOutput ); ++Idx )
				{
					Log::OutputBuffer Buffer = GLog->_BufferedOutput[Idx];

					_Printf( Buffer._LogChannel, "%s", Buffer._String );

					memory_globals::default_allocator().deallocate( Buffer._String );
				}
				array::clear( GLog->_BufferedOutput );
			}
		}
	}
}




// TODO: setup log in initialize if -log was passed as startup parameter
/*void Log::Initialize()
{
	Super::Initialize();

	mClock = new Time();

	String StartupString( Sys_GetCmdLine() );
	fIsConsoleInitialized = StartupString.ContainsWord( "-log" );

	if( fIsConsoleInitialized )
	{
		fSetupConsole();
	}
}*/

/*void Log::Finalize()
{
	Super::Finalize();

	if( fIsConsoleInitialized )
	{
		fReleaseConsole();
	}

	delete mClock;
}*/
/*
Log::Log() :
	mOutputStreams( foundation::memory_globals::default_allocator() )
{
}

void Log::AddOutputStream( OutputStream* Stream )
{
	foundation::array::push_back( mOutputStreams, Stream );
}

// TODO: Add logging to file
// TODO: Don't log to console if console wasn't initialized
void Log::Printf( const char* Format, ... )
{
	// @TODO: Move to constructor / destructor
	//fSetupConsole(); fReleaseConsole();
	if( fIsConsoleInitialized )
	{
		va_list ap;

		// @TODO: Add support for changeing the color of the log
		//SetConsoleTextAttribute( fOutputHandle, ( i++ % 2 ) ? 14 : 15 );	
	
		printf( "[%.2f] ", mClock->GetSecondsSinceStart() );

		va_start( ap, Format );
		vprintf( Format, ap );
		va_end( ap );

		printf( "%s", "\n" );
	}
}*/


