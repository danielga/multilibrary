#include <MultiLibrary/Network/SocketTCP.hpp>

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

SocketTCP::SocketTCP( IPAddressFamily family ) :
	Socket( family )
{
	Open( );
}

bool SocketTCP::Open( )
{
	if( IsValid( ) )
		return false;

	socket_id = socket( GetAddressFamily( ), SOCK_STREAM, IPPROTO_TCP );
	if( socket_id != INVALID_SOCKET )
	{
		char yes = 1;
		setsockopt( socket_id, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( yes ) );
	}

	return socket_id != INVALID_SOCKET;
}

SocketType SocketTCP::Type( ) const
{
	return TCP;
}

} // namespace MultiLibrary