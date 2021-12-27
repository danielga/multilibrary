/*************************************************************************
 * MultiLibrary - https://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2022, Daniel Almeida
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
 *************************************************************************/

#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <string>

struct sockaddr;
struct sockaddr_storage;
struct sockaddr_in;
struct sockaddr_in6;

namespace MultiLibrary
{

enum IPAddressFamily
{
	FAMILY_INET = 2,
	FAMILY_INET6 = 23
};

class MULTILIBRARY_NETWORK_API IPAddress
{
public:
	IPAddress( IPAddressFamily family = FAMILY_INET );

	IPAddress( const std::string &address, uint16_t port );

	IPAddress( const sockaddr_in &address );
	IPAddress( uint32_t address, uint16_t port );
	IPAddress( const uint8_t address[4], uint16_t port );

	IPAddress( const sockaddr_in6 &address );
	IPAddress( const uint16_t address[8], uint16_t port );

	virtual ~IPAddress( );

	IPAddress &operator=( const IPAddress &address );

	void SetIPAddress( const sockaddr_in &address );
	void SetIPAddress( const sockaddr_in6 &address );

	void SetAddress( const std::string &address );

	void SetAddress( uint32_t address );
	void SetAddress( const uint8_t address[4] );

	void SetAddress( const uint16_t address[8] );

	void SetPort( uint16_t port );

	uint32_t GetAddress( ) const;
	void GetAddress( uint8_t address[4] ) const;

	void GetAddress( uint16_t address[8] ) const;

	uint16_t GetPort( ) const;

	const sockaddr *ToSocketAddress( ) const;
	sockaddr *GetSocketAddress( );
	size_t GetAddressSize( ) const;

	std::string ToString( ) const;

	void SetFamily( IPAddressFamily family );
	IPAddressFamily GetFamily( ) const;

protected:
	void InitializeAddress( );
	bool ResolveString( const std::string &address );

	sockaddr_storage *host_address;
};

} // namespace MultiLibrary
