/*************************************************************************
 * MultiLibrary - http://danielga.github.io/multilibrary/
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

#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <MultiLibrary/Network/SocketError.hpp>
#include <MultiLibrary/Network/IPAddress.hpp>

namespace MultiLibrary
{

class ByteBuffer;

#if defined _WIN32

	typedef uintptr_t SocketHandle;

#elif defined __APPLE__ || defined __linux

	typedef int SocketHandle;
	
#endif

enum SocketType
{
	INVALID = -1,
	UDP,
	TCP
};

class MULTILIBRARY_NETWORK_API Socket
{
protected:
	Socket( IPAddressFamily family );
	virtual ~Socket( );

public:
	virtual SocketError Accept( Socket &socket, IPAddress &address );

	virtual SocketError Bind( const IPAddress &address );

	virtual bool CanRead( );
	virtual bool CanWrite( );

	virtual SocketError Close( );

	virtual SocketError Connect( const IPAddress &address );

	virtual IPAddressFamily GetAddressFamily( ) const;

	virtual bool IsValid( ) const;
	explicit operator bool( ) const;
	bool operator!( ) const;

	virtual SocketError Listen( int max_con_requests );

	virtual bool Open( );

	virtual SocketError Receive( void *buffer, int size, int flags, int *received_bytes = nullptr );
	virtual SocketError Receive( ByteBuffer &buffer, int flags );

	virtual SocketError ReceiveFrom( void *buffer, int size, int flags, IPAddress &address, int *received_bytes = nullptr );
	virtual SocketError ReceiveFrom( ByteBuffer &buffer, int flags, IPAddress &address );

	virtual std::string RemoteAddress( ) const;
	virtual uint16_t RemotePort( ) const;
	virtual IPAddress RemoteIPAddress( ) const;

	virtual SocketError Send( const void *buffer, int size, int flags, int *sent_bytes = nullptr );
	virtual SocketError Send( const ByteBuffer &buffer, int flags, int *sent_bytes = nullptr );

	virtual SocketError SendTo( const void *buffer, int size, int flags, const IPAddress &address, int *sent_bytes = nullptr );
	virtual SocketError SendTo( const ByteBuffer &buffer, int flags, const IPAddress &address, int *sent_bytes = nullptr );

	virtual SocketError SetBlocking( bool block );
	virtual SocketError SetTimeout( uint32_t timeout );

	virtual SocketError Shutdown( int flags );

	virtual SocketType Type( ) const;

protected:
	SocketHandle socket_id;
	IPAddressFamily address_family;
	bool is_blocking;
};

} // namespace MultiLibrary
