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

#include <MultiLibrary/Common/Linux/Pipe.hpp>
#include <system_error>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <errno.h>

namespace MultiLibrary
{

Pipe::Handle::Handle( int pipe ) :
	internal( pipe )
{ }

Pipe::Handle::~Handle( )
{
	if( internal != -1 )
		close( internal );
}

Pipe::Handle::operator int( ) const
{
	return internal;
}

Pipe::Pipe( Standard::Input in, Standard::Output out )
{
	int std_handle = -1;

	if( in == Standard::Input::Normal )
	{
		if( ( std_handle = dup( 0 ) ) == -1 )
			throw std::system_error( errno, std::system_category( ), "unable to duplicate standard input handle" );

		read_handle.reset( new Handle( std_handle ) );
	}

	switch( out )
	{
		case Standard::Output::Normal:
			if( ( std_handle = dup( 1 ) ) == -1 )
				throw std::system_error( errno, std::system_category( ), "unable to duplicate standard output handle" );

			write_handle.reset( new Handle( std_handle ) );
			break;

		case Standard::Output::Error:
			if( ( std_handle = dup( 2 ) ) == -1 )
				throw std::system_error( errno, std::system_category( ), "unable to duplicate standard error handle" );

			write_handle.reset( new Handle( std_handle ) );
			break;

		default:
			break;
	}
}

Pipe::Pipe( bool, bool )
{
	int handles[2] = { -1, -1 };
	if( pipe( handles ) != 0 )
		throw std::system_error( errno, std::system_category( ), "unable to create pipe" );

	fcntl( handles[0], F_SETFD, FD_CLOEXEC );
	fcntl( handles[1], F_SETFD, FD_CLOEXEC );
	read_handle.reset( new Handle( handles[0] ) );
	write_handle.reset( new Handle( handles[1] ) );
}

Pipe::Pipe( Pipe &&pipe ) noexcept :
	read_handle( std::move( pipe.read_handle ) ),
	write_handle( std::move( pipe.write_handle ) )
{ }

Pipe::~Pipe( )
{
	CloseRead( );
	CloseWrite( );
}

bool Pipe::IsValid( ) const
{
	return read_handle || write_handle;
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
	if( !read_handle )
		return 0;

	int size = 0;
	if( ioctl( *read_handle, FIONREAD, &size ) == 0 )
		return 0;

	return 0;
}

bool Pipe::EndOfFile( ) const
{
	return static_cast<bool>( read_handle );
}

size_t Pipe::Read( void *data, size_t size )
{
	if( !read_handle )
		return 0;

	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( *read_handle, &rfds );
	timeval time = { 0, 0 };
	if( select( 1, &rfds, nullptr, nullptr, &time ) == 1 )
	{
		ssize_t res = read( *read_handle, data, size );
		if( res >= 0 )
			return res;
	}

	return 0;
}

size_t Pipe::Write( const void *data, size_t size )
{
	if( !write_handle )
		return 0;

	fd_set rfds;
	FD_ZERO( &rfds );
	FD_SET( *write_handle, &rfds );
	timeval time = { 0, 0 };
	if( select( 1, nullptr, &rfds, nullptr, &time ) == 1 )
	{
		ssize_t res = write( *write_handle, data, size );
		if( res >= 0 )
			return res;
	}

	return 0;
}

void Pipe::CloseRead( )
{
	read_handle.reset( );
}

void Pipe::CloseWrite( )
{
	write_handle.reset( );
}

const Pipe::Handle &Pipe::ReadHandle( ) const
{
	return *read_handle.get( );
}

const Pipe::Handle &Pipe::WriteHandle( ) const
{
	return *write_handle.get( );
}

} // namespace MultiLibrary
