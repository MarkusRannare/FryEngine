#include "SocketSubsystem.h"
#include "Log.h"
#include <cassert>

using namespace FryCore;

IMPLEMENT_CLASS( SocketSubsystem );

static bool fSocketInitialized = FALSE;
static bool fPendingDestroy = FALSE;

extern Log* g_Log;

void SocketSubsystem::StaticInitialize()
{
	const int RequiredSocketVersion = 2;
	WSADATA WsaData;

	int ErrorCode = WSAStartup( MAKEWORD( RequiredSocketVersion, 0 ),  &WsaData );
	if( ErrorCode == 0 )
	{
		// Startup Succeeded
		fSocketInitialized = TRUE;
	}
	else
	{
		// Startup FAILED
		g_Log->Printf( "Failed to startup the socket subsystem with ErrorCode %i", ErrorCode );
	}
}

void SocketSubsystem::StaticFinalize()
{
	int ErrorCode = -1;
	if( fSocketInitialized )
	{
		ErrorCode = WSACleanup();
	}
	if( ErrorCode != 0 )
	{
		g_Log->Printf( "Failed to shutdown the socket subsystem with error code %i", ErrorCode );
	}
}

void SocketSubsystem::Initialize()
{
	fPendingDestroy = false;

	//Sys_CreateThread( &SocketSubsystem::ThreadEntry, "Listening thread", mThreadInfo, this );
}

void SocketSubsystem::Finalize()
{
	fPendingDestroy = true;

	mListeningMutex.Aquire();
	
	for( size_t Idx = 0; Idx < mThreadInfo.Length(); ++Idx )
	{
		Sys_DestoryThread( *mThreadInfo[Idx] );
		delete mThreadInfo[Idx];
	}
	//mThreadInfo.clear();

	mListeningMutex.Release();
}

void SocketSubsystem::SetupListenSocket( Socket* ListeningSocket )
{
	ThreadStartupData ThreadData = { ListeningSocket, false };

	mListeningMutex.Aquire();
	mThreadStartupData.AddItem( ThreadData );
	
	ThreadInfo* NewThreadInfo = new ThreadInfo;
	mThreadInfo.AddItem( NewThreadInfo );

	Sys_CreateThread( &SocketSubsystem::ListeningThread, "Listening thread", *mThreadInfo[mThreadInfo.Length()-1], this, TCF_CREATE_RUNNING );

	mListeningMutex.Release();
}

void SocketSubsystem::Tick( float DeltaTime )
{
	UNUSED( DeltaTime );

	mConnectionMutex.Aquire();
	for( size_t i = 0; i < mNewConnections.Length(); ++i )
	{
		g_Log->Printf( "Got a new connection" );
		mConnections.AddItem( mNewConnections[i] );
	}
	mConnectionMutex.Release();
}

unsigned long THREAD_FUNC SocketSubsystem::ListeningThread( void* Arg )
{
	SocketSubsystem* Subsystem = (SocketSubsystem*)Arg;

	ThreadStartupData StartupData;
	memset( &StartupData, 0, sizeof( StartupData ) );
	Subsystem->mListeningMutex.Aquire();
	Array<ThreadStartupData>& StartupDataList = Subsystem->mThreadStartupData;
	for( size_t i = 0; i < StartupDataList.Length(); ++i )
	{
		if( !StartupDataList[i].IsClaimed )
		{
			StartupDataList[i].IsClaimed = TRUE;
			StartupData = StartupDataList[i];
		}
	}
	Subsystem->mListeningMutex.Release();

	assert( StartupData.IsClaimed && StartupData.Socket != NULL && "Couldn't find startup data for socket listening thread" );

	Socket* ListeningSocket = StartupData.Socket;

	UBOOL ContinueListening = TRUE;
	while( ContinueListening && !fPendingDestroy )
	{
		// Blocking call
		ListeningSocket->Listen();
		
		// If we get here, we have gotten a new connection, accept it
		Socket* NewSocket = ListeningSocket->Accept();
		
		if( NewSocket != NULL && !fPendingDestroy )
		{
			Subsystem->mConnectionMutex.Aquire();
			Subsystem->mNewConnections.AddItem( NewSocket );
			Subsystem->mConnectionMutex.Release();
		}
		else if( NewSocket != NULL )
		{
			// CLEANUP, we allocated a socket while shutting down
			delete NewSocket;
		}

		Sleep( 10 );
	}

	return 0;
}

Socket* SocketSubsystem::CreateDatagramSocket()
{
	return new Socket( Socket::AF_IPV4, Socket::P_UDP );
}

Socket* SocketSubsystem::CreateStreamSocket()
{
	return new Socket( Socket::AF_IPV4, Socket::P_TCP );
}