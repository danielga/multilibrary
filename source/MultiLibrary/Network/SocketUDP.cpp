/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
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

#include <MultiLibrary/Network/SocketUDP.hpp>

#if defined _WIN32

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