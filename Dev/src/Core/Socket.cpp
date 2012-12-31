#include "Socket.h"
#include "Log.h"

using namespace FryCore;

extern Log* g_Log;

static unsigned long fNonBlocking = 1;

Socket::Socket( AddressFamily AddressFamily, Protocol Protocol ) :
	mAddressFamily( AddressFamily ),
	mProtocol( Protocol )
{
	mSocket = socket( (int)AddressFamily, Protocol == P_TCP ? SOCK_STREAM : SOCK_DGRAM, Protocol == P_TCP ? IPPROTO_TCP : IPPROTO_UDP );
	
	ioctlsocket( mSocket, FIONBIO, &fNonBlocking );
}

Socket::Socket( SOCKET FromSocket, AddressFamily AddressFamily, Protocol Protocol ) :
	mSocket( FromSocket ),
	mAddressFamily( AddressFamily ),
	mProtocol( Protocol )
{
	ioctlsocket( mSocket, FIONBIO, &fNonBlocking );
}

Socket::~Socket()
{
	closesocket( mSocket );
}

void Socket::Bind( short Port )
{
	sockaddr_in SocketAddress;

	SocketAddress.sin_family = (short)mAddressFamily;
	SocketAddress.sin_port = htons( Port );
	SocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

	int ErrorCode = bind( mSocket, (sockaddr*)&SocketAddress, sizeof(SocketAddress) );
	if( ErrorCode != 0 )
	{
		g_Log->Printf( "Failed bind the socket to port %i with error code %i", Port, ErrorCode );
	}
}

void Socket::Listen( int MaxConnectionQueSize /* = SOMAXCONN */ )
{
	int ErrorCode = listen( mSocket, MaxConnectionQueSize );
	if( ErrorCode != 0 )
	{
		g_Log->Printf( "Failed to listen to socket with error code %i", ErrorCode );
	}
}

Socket* Socket::Accept()
{
	sockaddr_in RemoteAddress;
	int RemoteAddressLength = sizeof(RemoteAddress);

	SOCKET RemoteSocket;

	RemoteSocket = accept( mSocket, (sockaddr*)&RemoteAddress, &RemoteAddressLength );
	if( RemoteSocket != INVALID_SOCKET )
	{
		return new Socket( RemoteSocket, mAddressFamily, mProtocol );
	}

	g_Log->Printf( "Failed to accept connection" );

	return NULL;
}

Socket::ErrorCode Socket::ReceiveFrom( u8* Buffer, int BufferSize, sockaddr* From, int* FromSize )
{
	ErrorCode Error;
	Error = recvfrom( mSocket, (s8*)Buffer, BufferSize, 0, From, FromSize ) == ERROR_SUCCESS ? EC_NO_ERROR : EC_ERROR;
	
	return Error;
}