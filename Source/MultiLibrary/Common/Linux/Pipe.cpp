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

#include <MultiLibrary/Common/Pipe.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>

namespace MultiLibrary
{

Pipe::Pipe( void *read, void *write ) :
	read_handle( read ),
	write_handle( write )
{ }

Pipe::Pipe( bool, bool ) :
	read_handle( 0 ),
	write_handle( 0 )
{
	int handles[2] = { 0, 0 };
	if( pipe( handles ) == 0 )
	{
		fcntl( handles[0], F_SETFD, FD_CLOEXEC );
		fcntl( handles[1], F_SETFD, FD_CLOEXEC );
		read_handle = reinterpret_cast<void *>( handles[0] );
		write_handle = reinterpret_cast<void *>( handles[1] );
	}
}

Pipe::~Pipe( )
{
	CloseRead( );
	CloseWrite( );
}

bool Pipe::IsValid( ) const
{
	return read_handle != 0 || write_handle != 0;
}

bool Pipe::Seek( int64_t, SeekMode )
{
	return false;
}

int64_t Pipe::Tell( ) const
{
	return 0;
}

int64_t Pipe::Size( ) const
{
	int size = 0;
	if( ioctl( *reinterpret_cast<const int *>( &read_handle ), FIONREAD, &size ) == 0 )
		return 0;

	return 0;
}

bool Pipe::EndOfFile( ) const
{
	return read_handle == 0;
}

size_t Pipe::Read( void *data, size_t size )
{
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( *reinterpret_cast<int *>( &read_handle ), &rfds );
	timeval time = { 0, 0 };
	if( select( 1, &rfds, nullptr, nullptr, &time ) == 1 )
	{
		ssize_t res = read( *reinterpret_cast<int *>( &read_handle ), data, size );
		if( res >= 0 )
			return res;
	}

	return 0;
}

size_t Pipe::Write( const void *data, size_t size )
{
	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( *reinterpret_cast<int *>( &write_handle ), &rfds );
	timeval time = { 0, 0 };
	if( select( 1, nullptr, &rfds, nullptr, &time ) == 1 )
	{
		ssize_t res = write( *reinterpret_cast<int *>( &write_handle ), data, size );
		if( res >= 0 )
			return res;
	}

	return 0;
}

void *Pipe::ReadHandle( ) const
{
	return read_handle;
}

void *Pipe::WriteHandle( ) const
{
	return write_handle;
}

void Pipe::CloseRead( )
{
	if( read_handle != 0 )
	{
		close( *reinterpret_cast<int *>( &read_handle ) );
		read_handle = 0;
	}
}

void Pipe::CloseWrite( )
{
	if( write_handle != 0 )
	{
		close( *reinterpret_cast<int *>( &write_handle ) );
		write_handle = 0;
	}
}

} // namespace MultiLibrary
