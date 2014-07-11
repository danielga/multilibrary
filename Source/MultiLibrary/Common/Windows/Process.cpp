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

#include <MultiLibrary/Common/Process.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <stdexcept>
#include <windows.h>

namespace MultiLibrary
{

namespace Internal
{

static const size_t STREAM_BUFFER_SIZE = 4096;

class Pipe : public NonCopyable
{
public:
	Pipe( ) :
		read_handle( nullptr ),
		write_handle( nullptr )
	{ }

	~Pipe( )
	{
		Close( );
	}

	void Open( )
	{
		if( read_handle != nullptr || write_handle != nullptr )
			return;

		SECURITY_ATTRIBUTES sa;
		sa.nLength = static_cast<DWORD>( sizeof( SECURITY_ATTRIBUTES ) );
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = nullptr;
		if( CreatePipe( &read_handle, &write_handle, &sa, 0 ) == FALSE )
			throw std::runtime_error( "unable to create pipe" );
	}

	void CloseRead( )
	{
		if( read_handle != nullptr )
		{
			if( CloseHandle( read_handle ) == FALSE )
				throw std::runtime_error( "unable to close pipe read handle" );

			read_handle = nullptr;
		}
	}

	void CloseWrite( )
	{
		if( write_handle != nullptr )
		{
			if( CloseHandle( write_handle ) == FALSE )
				throw std::runtime_error( "unable to close pipe write handle" );

			write_handle = nullptr;
		}
	}

	void Close( )
	{
		CloseRead( );
		CloseWrite( );
	}

	HANDLE Read( ) const
	{
		return read_handle;
	}

	HANDLE Write( ) const
	{
		return write_handle;
	}

private:
	HANDLE read_handle;
	HANDLE write_handle;
};

class PipeReadStream : public std::streambuf, public NonCopyable
{
public:
	PipeReadStream( Pipe &pipe_ref ) :
		pipe( pipe_ref )
	{ }

protected:
	virtual int_type underflow( )
	{
		if( pipe.Read( ) != nullptr && gptr( ) == egptr( ) )
		{
			DWORD size = 0;
			if( PeekNamedPipe( pipe.Read( ), nullptr, 0, nullptr, &size, nullptr ) == FALSE || size == 0 )
				return traits_type::eof( );

			if( ReadFile( pipe.Read( ), stream_buffer, STREAM_BUFFER_SIZE, &size, nullptr ) == FALSE || size == 0 )
				return traits_type::eof( );

			setg( stream_buffer, stream_buffer, stream_buffer + size );
			return traits_type::to_int_type( *eback( ) );
		}

		return traits_type::eof( );
	}

private:
	Pipe &pipe;
	char stream_buffer[STREAM_BUFFER_SIZE];
};

class PipeWriteStream : public std::streambuf, public NonCopyable
{
public:
	PipeWriteStream( Pipe &pipe_ref ) :
		pipe( pipe_ref )
	{
		setp( stream_buffer, stream_buffer + STREAM_BUFFER_SIZE );
	}

protected:
	virtual int sync( )
	{
		if( pipe.Write( ) != nullptr && pbase( ) != pptr( ) )
		{
			DWORD size = static_cast<DWORD>( pptr( ) - pbase( ) ), written = 0;
			if( WriteFile( pipe.Write( ), pbase( ), size, &written, nullptr ) == TRUE && written == size )
			{
				setp( stream_buffer, stream_buffer + STREAM_BUFFER_SIZE );
				return 0;
			}
		}

		return -1;
	}

	virtual int_type overflow( int_type c = traits_type::eof( ) )
	{
		if( sync( ) == -1 )
			return traits_type::eof( );

		if( c != traits_type::eof( ) && sputc( traits_type::to_char_type( c ) ) == traits_type::eof( ) )
			return traits_type::eof( );

		return traits_type::not_eof( c );
	}

private:
	Pipe &pipe;
	char stream_buffer[STREAM_BUFFER_SIZE];
};

} // namespace Internal

struct Process::InternalData : public NonCopyable
{
	InternalData( ) :
		handle( nullptr ),
		input_streambuf( input_pipe ),
		output_streambuf( output_pipe ),
		error_streambuf( error_pipe ),
		input_stream( &input_streambuf ),
		output_stream( &output_streambuf ),
		error_stream( &error_streambuf )
	{
		input_pipe.Open( );
		output_pipe.Open( );
		error_pipe.Open( );

		if( SetHandleInformation( input_pipe.Write( ), HANDLE_FLAG_INHERIT, 0 ) == FALSE )
			throw std::runtime_error( "unable to set pipe read handle flag" );

		if( SetHandleInformation( output_pipe.Read( ), HANDLE_FLAG_INHERIT, 0 ) == FALSE )
			throw std::runtime_error( "unable to set pipe read handle flag" );

		if( SetHandleInformation( error_pipe.Read( ), HANDLE_FLAG_INHERIT, 0 ) == FALSE )
			throw std::runtime_error( "unable to set pipe read handle flag" );
	}

	HANDLE handle;

	Internal::Pipe input_pipe;
	Internal::Pipe output_pipe;
	Internal::Pipe error_pipe;

	Internal::PipeWriteStream input_streambuf;
	Internal::PipeReadStream output_streambuf;
	Internal::PipeReadStream error_streambuf;

	std::ostream input_stream;
	std::istream output_stream;
	std::istream error_stream;
};

Process::Process( ) :
	process( nullptr ),
	exit_code( 0 )
{ }

Process::~Process( )
{
	Close( );
}

bool Process::Start( const std::string &path, const std::string &args )
{
	if( process != nullptr )
		return false;

	std::string cmdline;
	cmdline.reserve( path.size( ) + 3 + args.size( ) );
	cmdline += '\"';
	cmdline += path;
	cmdline += '\"';

	if( !args.empty( ) )
	{
		cmdline += ' ';
		cmdline += args;
	}

	std::wstring widecmdline;
	UTF16::FromUTF8( cmdline.begin( ), cmdline.end( ), std::back_inserter( widecmdline ) );

	process = new InternalData;

	STARTUPINFO startup;
	ZeroMemory( &startup, sizeof( startup ) );
	startup.cb = sizeof( startup );
	startup.dwFlags = STARTF_USESTDHANDLES;
	startup.hStdInput = process->input_pipe.Read( );
	startup.hStdOutput = process->output_pipe.Write( );
	startup.hStdError = process->error_pipe.Write( );

	PROCESS_INFORMATION info;

	if( CreateProcess( nullptr, &widecmdline[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup, &info ) == TRUE )
	{
		if( CloseHandle( info.hThread ) == FALSE )
			throw std::runtime_error( "unable to close thread handle" );

		process->input_pipe.CloseRead( );
		process->output_pipe.CloseWrite( );
		process->error_pipe.CloseWrite( );
		process->handle = info.hProcess;
		exit_code = 0;
		return true;
	}

	delete process;
	process = nullptr;
	return false;
}

bool Process::Start( const std::string &path, const std::vector<std::string> &args )
{
	if( process != nullptr )
		return false;

	std::string targs;

	std::vector<std::string>::const_iterator it, begin = args.begin( ), end = args.end( );
	for( it = begin; it != end; ++it )
	{
		targs.reserve( targs.size( ) + ( *it ).size( ) + 3 );

		if( it != begin )
			targs += ' ';

		targs += '\"';
		targs += *it;
		targs += '\"';
	}

	return Start( path, targs );
}

bool Process::Wait( const std::chrono::microseconds &timeout )
{
	if( process == nullptr )
		return false;

	std::chrono::milliseconds millisecs = std::chrono::duration_cast<std::chrono::milliseconds>( timeout );
	return WaitForSingleObject( process->handle, static_cast<DWORD>( millisecs.count( ) ) ) == WAIT_OBJECT_0;
}

bool Process::Close( )
{
	if( process == nullptr )
		return false;

	CloseInput( );

	if( WaitForSingleObject( process->handle, INFINITE ) == WAIT_OBJECT_0 )
	{
		DWORD code = 0;
		if( GetExitCodeProcess( process->handle, &code ) == FALSE )
			throw std::runtime_error( "unable to get process exit code" );

		exit_code = static_cast<int32_t>( code );

		if( CloseHandle( process->handle ) == FALSE )
			throw std::runtime_error( "unable to close process handle" );

		delete process;
		process = nullptr;
		return true;
	}

	return false;
}

void Process::Kill( )
{
	if( process == nullptr )
		return;

	if( TerminateProcess( process->handle, 0 ) == FALSE || !Close( ) )
		throw std::runtime_error( "unable to terminate process" );
}

void Process::CloseInput( )
{
	if( process != nullptr && process->input_pipe.Write( ) != nullptr )
	{
		process->input_stream.flush( );
		process->input_pipe.CloseWrite( );
	}
}

std::ostream &Process::Input( )
{
	return process->input_stream;
}

std::istream &Process::Output( )
{
	return process->output_stream;
}

std::istream &Process::Error( )
{
	return process->error_stream;
}

int32_t Process::ExitCode( ) const
{
	return exit_code;
}

} // namespace MultiLibrary