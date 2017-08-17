/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2017, Daniel Almeida
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <MultiLibrary/Network/Socket.hpp>
#include <MultiLibrary/Common/ByteBuffer.hpp>

#if defined _WIN32

	#include <winsock2.h>
	#include <ws2tcpip.h>
	#include <stdexcept>
	#define GetSocketError( ) WSAGetLastError( )

#elif defined __APPLE__ || defined __linux

	#include <unistd.h>
	#include <cstring>
	#include <errno.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/ioctl.h>
	#define INVALID_SOCKET ( -1 )
	#define SOCKET_ERROR ( -1 )
	#define GetSocketError( ) errno
	#define closesocket close
	#define ioctlsocket ioctl

#endif

namespace MultiLibrary
{

#if defined _WIN32

namespace Internal
{

class WSAInitializer
{
public:
	WSAInitializer( )
	{
		WSADATA wsa_data;
		if( WSAStartup( MAKEWORD( 2, 2 ), &wsa_data ) != 0 )
			throw std::runtime_error( "WSAStartup failed" );
		else if( LOBYTE( wsa_data.wVersion ) != 2 || HIBYTE( wsa_data.wVersion ) != 2 )
			throw std::runtime_error( "could not find a usable version of winsock.dll" );
	}

	~WSAInitializer( )
	{
		WSACleanup( );
	}
};

static WSAInitializer wsa_init;

}

#endif // _WIN32

Socket::Socket( IPAddressFamily family ) :
	socket_id( INVALID_SOCKET ),
	address_family( family ),
	is_blocking( true )
{ }

Socket::~Socket( )
{
	Close( );
}

SocketError Socket::Accept( Socket &socket, IPAddress &address )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	address.SetFamily( address_family );
	socklen_t address_size = static_cast<socklen_t>( address.GetAddressSize( ) );

	SocketHandle newSocket = accept( socket_id, address.GetSocketAddress( ), &address_size );
	if( newSocket != INVALID_SOCKET )
		socket.socket_id = newSocket;

	return newSocket == INVALID_SOCKET ? GetSocketError( ) : 0;
}

SocketError Socket::Bind( const IPAddress &address )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	int ret = bind( socket_id, address.ToSocketAddress( ), static_cast<socklen_t>( address.GetAddressSize( ) ) );
	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

bool Socket::CanRead( )
{
	if( !IsValid( ) )
		return false;

	fd_set set;
	FD_ZERO( &set );
	FD_SET( socket_id, &set );
	timeval wait = { 0, 0 };
	return select( static_cast<int>( socket_id ) + 1, &set, 0, 0, &wait ) > 0;
}

bool Socket::CanWrite( )
{
	if( !IsValid( ) )
		return false;

	fd_set set;
	FD_ZERO( &set );
	FD_SET( socket_id, &set );
	timeval wait = { 0, 0 };
	return select( static_cast<int>( socket_id ) + 1, 0, &set, 0, &wait ) > 0;
}

SocketError Socket::Close( )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	Shutdown( 2 );

	int ret = closesocket( socket_id );
	socket_id = INVALID_SOCKET;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::Connect( const IPAddress &address )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	int ret = connect( socket_id, address.ToSocketAddress( ), static_cast<socklen_t>( address.GetAddressSize( ) ) );
	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

IPAddressFamily Socket::GetAddressFamily( ) const
{
	return address_family;
}

bool Socket::IsValid( ) const
{
	return socket_id != INVALID_SOCKET;
}

Socket::operator bool( ) const
{
	return IsValid( );
}

bool Socket::operator!( ) const
{
	return !IsValid( );
}

SocketError Socket::Listen( int max_con_requests )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	return listen( socket_id, max_con_requests ) == SOCKET_ERROR ? GetSocketError( ) : 0;
}

bool Socket::Open( )
{
	return false;
}

SocketError Socket::Receive( void *buffer, int size, int flags, int *received_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	int ret = recv( socket_id, static_cast<char *>( buffer ), size, flags );
	if( ret != SOCKET_ERROR && received_bytes != nullptr )
		*received_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::Receive( ByteBuffer &buffer, int flags )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	char *buf = reinterpret_cast<char *>( buffer.GetBuffer( ) );

	int ret = recv( socket_id, buf, static_cast<int>( buffer.Size( ) ), flags );
	if( ret != SOCKET_ERROR )
		buffer.Resize( ret );

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::ReceiveFrom( void *buffer, int size, int flags, IPAddress &address, int *received_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	address.SetFamily( address_family );
	sockaddr *addr = address.GetSocketAddress( );
	socklen_t addr_size = static_cast<socklen_t>( address.GetAddressSize( ) );

	int ret = recvfrom( socket_id, static_cast<char *>( buffer ), size, flags, addr, &addr_size );
	if( ret != SOCKET_ERROR && received_bytes != nullptr )
		*received_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::ReceiveFrom( ByteBuffer &buffer, int flags, IPAddress &address )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	char *buf = reinterpret_cast<char *>( buffer.GetBuffer( ) );
	address.SetFamily( address_family );
	sockaddr *addr = address.GetSocketAddress( );
	socklen_t addr_size = static_cast<socklen_t>( address.GetAddressSize( ) );

	int ret = recvfrom( socket_id, buf, static_cast<int>( buffer.Size( ) ), flags, addr, &addr_size );
	if( ret != SOCKET_ERROR )
		buffer.Resize( ret );

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

std::string Socket::RemoteAddress( ) const
{
	if( !IsValid( ) )
		return "";

	return RemoteIPAddress( ).ToString( );
}

uint16_t Socket::RemotePort( ) const
{
	if( !IsValid( ) )
		return 0;

	return RemoteIPAddress( ).GetPort( );
}

IPAddress Socket::RemoteIPAddress( ) const
{
	if( !IsValid( ) )
		return IPAddress( );

	IPAddress addr( address_family );
	socklen_t addr_size = static_cast<socklen_t>( addr.GetAddressSize( ) );

	if( getpeername( socket_id, addr.GetSocketAddress( ), &addr_size ) != SOCKET_ERROR )
		return addr;

	return IPAddress( );
}

SocketError Socket::Send( const void *buffer, int size, int flags, int *sent_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	int ret = send( socket_id, static_cast<const char *>( buffer ), size, flags );
	if( ret != SOCKET_ERROR && sent_bytes != nullptr )
		*sent_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::Send( const ByteBuffer &buffer, int flags, int *sent_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	const char *buf = reinterpret_cast<const char *>( buffer.GetBuffer( ) );

	int ret = send( socket_id, buf, static_cast<int>( buffer.Size( ) ), flags );
	if( ret != SOCKET_ERROR && sent_bytes != nullptr )
		*sent_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::SendTo( const void *buffer, int size, int flags, const IPAddress &address, int *sent_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	const sockaddr *addr = address.ToSocketAddress( );
	socklen_t addr_size = static_cast<socklen_t>( address.GetAddressSize( ) );

	int ret = sendto( socket_id, static_cast<const char *>( buffer ), size, flags, addr, addr_size );
	if( ret != SOCKET_ERROR && sent_bytes != nullptr )
		*sent_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::SendTo( const ByteBuffer &buffer, int flags, const IPAddress &address, int *sent_bytes )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	const char *buf = reinterpret_cast<const char *>( buffer.GetBuffer( ) );
	const sockaddr *addr = address.ToSocketAddress( );
	socklen_t addr_size = static_cast<socklen_t>( address.GetAddressSize( ) );

	int ret = sendto( socket_id, buf, static_cast<int>( buffer.Size( ) ), flags, addr, addr_size );
	if( ret != SOCKET_ERROR && sent_bytes != nullptr )
		*sent_bytes = ret;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::SetBlocking( bool block )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	u_long shouldblock = block ? 0 : 1;
	int ret = ioctlsocket( socket_id, FIONBIO, &shouldblock );
	if( ret != SOCKET_ERROR )
		is_blocking = block;

	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::SetTimeout( uint32_t timeout )
{
	if( !IsValid( ) )
		return ENOTSOCK;

#if defined _WIN32

	uint32_t time = timeout;

#else

	struct timeval time;
	time.tv_sec = timeout % 1000;
	time.tv_usec = ( timeout - time.tv_sec * 1000 ) * 1000;
	
#endif

	const char *t = reinterpret_cast<const char *>( &time );
	int ret = setsockopt( socket_id, SOL_SOCKET, SO_RCVTIMEO, t, sizeof( time ) );
	return ret == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketError Socket::Shutdown( int flags )
{
	if( !IsValid( ) )
		return ENOTSOCK;

	return shutdown( socket_id, flags ) == SOCKET_ERROR ? GetSocketError( ) : 0;
}

SocketType Socket::Type( ) const
{
	return INVALID;
}

} // namespace MultiLibrary
