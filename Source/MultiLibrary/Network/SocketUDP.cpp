#include <MultiLibrary/Network/SocketUDP.hpp>

#if defined _WIN32

	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <ws2tcpip.h>

#elif defined __APPLE__ || defined __linux

	#include <errno.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#define INVALID_SOCKET ( -1 )

#endif

namespace MultiLibrary
{

SocketUDP::SocketUDP( IPAddressFamily family ) :
	Socket( family )
{
	Open( );
}

SocketError SocketUDP::Listen( const int )
{
	return ENOTSUP;
}

bool SocketUDP::Open( )
{
	if( IsValid( ) )
		return false;

	socket_id = socket( GetAddressFamily( ), SOCK_DGRAM, IPPROTO_UDP );
	if( socket_id != INVALID_SOCKET )
	{
		char yes = 1;
		setsockopt( socket_id, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) );
	}

	return socket_id != INVALID_SOCKET;
}

SocketType SocketUDP::Type( ) const
{
	return UDP;
}

} // namespace MultiLibrary