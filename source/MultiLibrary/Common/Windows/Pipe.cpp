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

#include <MultiLibrary/Common/Windows/Pipe.hpp>
#include <stdexcept>
#include <system_error>

namespace MultiLibrary
{

Pipe::Handle::Handle( HANDLE proc ) :
	handle( proc )
{ }

Pipe::Handle::~Handle( )
{
	if( handle != nullptr )
		CloseHandle( handle );
}

Pipe::Handle::operator HANDLE( ) const
{
	return handle;
}

Pipe::Pipe( Standard::Input in, Standard::Output out )
{
	HANDLE cur_proc = GetCurrentProcess( );
	HANDLE std_handle = nullptr;
	if( in == Standard::Input::Normal )
	{
		std_handle = GetStdHandle( STD_INPUT_HANDLE );
		if( std_handle == INVALID_HANDLE_VALUE || std_handle == nullptr )
			throw std::system_error( GetLastError( ), std::system_category( ), "unable to obtain standard input handle" );

		if( DuplicateHandle(
			cur_proc,
			std_handle,
			cur_proc,
			&std_handle,
			0,
			FALSE,
			DUPLICATE_SAME_ACCESS
		) == FALSE )
			throw std::system_error( GetLastError( ), std::system_category( ), "unable to duplicate standard input handle" );

		read_handle.reset( new Handle( std_handle ) );
	}

	switch( out )
	{
		case Standard::Output::Normal:
			std_handle = GetStdHandle( STD_OUTPUT_HANDLE );
			if( std_handle == INVALID_HANDLE_VALUE || std_handle == nullptr )
				throw std::system_error( GetLastError( ), std::system_category( ), "unable to obtain standard output handle" );

			if( DuplicateHandle(
				cur_proc,
				std_handle,
				cur_proc,
				&std_handle,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			) == FALSE )
				throw std::system_error( GetLastError( ), std::system_category( ), "unable to duplicate standard output handle" );

			write_handle.reset( new Handle( std_handle ) );
			break;

		case Standard::Output::Error:
			std_handle = GetStdHandle( STD_ERROR_HANDLE );
			if( std_handle == INVALID_HANDLE_VALUE || std_handle == nullptr )
				throw std::system_error( GetLastError( ), std::system_category( ), "unable to obtain standard error handle" );

			if( DuplicateHandle(
				cur_proc,
				std_handle,
				cur_proc,
				&std_handle,
				0,
				FALSE,
				DUPLICATE_SAME_ACCESS
			) == FALSE )
				throw std::system_error( GetLastError( ), std::system_category( ), "unable to duplicate standard error handle" );

			write_handle.reset( new Handle( std_handle ) );
			break;

		default:
			break;
	}
}

Pipe::Pipe( bool read_inheritable, bool write_inheritable )
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = static_cast<DWORD>( sizeof( SECURITY_ATTRIBUTES ) );
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = nullptr;

	HANDLE read = nullptr, write = nullptr;
	if( CreatePipe( &read, &write, &sa, 0 ) == FALSE )
		throw std::system_error( GetLastError( ), std::system_category( ), "unable to create pipe" );

	if( !read_inheritable && SetHandleInformation( read, HANDLE_FLAG_INHERIT, FALSE ) == FALSE )
		throw std::system_error( GetLastError( ), std::system_category( ), "unable to set pipe write handle flag" );

	if( !write_inheritable && SetHandleInformation( write, HANDLE_FLAG_INHERIT, FALSE ) == FALSE )
		throw std::system_error( GetLastError( ), std::system_category( ), "unable to set pipe read handle flag" );

	read_handle.reset( new Handle( read ) );
	write_handle.reset( new Handle( write ) );
}

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

	DWORD avail = 0;
	PeekNamedPipe( *read_handle, nullptr, 0, nullptr, &avail, nullptr );
	return avail;
}

bool Pipe::EndOfFile( ) const
{
	return static_cast<bool>( read_handle );
}

size_t Pipe::Read( void *data, size_t size )
{
	if( !read_handle )
		return 0;

	DWORD avail = static_cast<DWORD>( Size( ) );
	if( avail > 0 && ReadFile( *read_handle, data, avail < size ? avail : static_cast<DWORD>( size ), &avail, nullptr ) == FALSE )
		return 0;

	return avail;
}

size_t Pipe::Write( const void *data, size_t size )
{
	if( !write_handle )
		return 0;

	DWORD written = 0;
	WriteFile( *write_handle, data, static_cast<DWORD>( size ), &written, nullptr );
	return written;
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
