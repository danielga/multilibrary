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

#include <MultiLibrary/Common/Process.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <MultiLibrary/Common/Windows/Pipe.hpp>
#include <system_error>
#include <iterator>
#include <windows.h>

namespace MultiLibrary
{

class Process::Handle
{
public:
	Handle( HANDLE proc ) :
		handle( proc )
	{ }

	~Handle( )
	{
		if( handle != nullptr )
			CloseHandle( handle );
	}

	operator HANDLE( ) const
	{
		return handle;
	}

private:
	HANDLE handle;
};

Process::Process( const std::string &path, const std::vector<std::string> &args ) :
	exit_code( 0 ),
	input_pipe( true, false ),
	output_pipe( false, true ),
	error_pipe( false, true )
{
	std::string cmdline;
	cmdline.reserve( path.size( ) + 3 );
	cmdline += '\"';
	cmdline += path;
	cmdline += '\"';

	std::vector<std::string>::const_iterator it, end = args.end( );
	for( it = args.begin( ); it != end; ++it )
	{
		cmdline += ' ';
		cmdline += '\"';
		cmdline += *it;
		cmdline += '\"';
	}

	std::wstring widecmdline;
	UTF16::FromUTF8( cmdline.begin( ), cmdline.end( ), std::back_inserter( widecmdline ) );

	STARTUPINFO startup;
	ZeroMemory( &startup, sizeof( startup ) );
	startup.cb = sizeof( startup );
	startup.dwFlags = STARTF_USESTDHANDLES;
	startup.hStdInput = input_pipe.ReadHandle( );
	startup.hStdOutput = output_pipe.WriteHandle( );
	startup.hStdError = error_pipe.WriteHandle( );

	PROCESS_INFORMATION info;
	if( CreateProcess( nullptr, &widecmdline[0], nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup, &info ) == FALSE )
		throw std::system_error( GetLastError( ), std::system_category( ), "failed to create process" );

	CloseHandle( info.hThread );
	process.reset( new Handle( info.hProcess ) );
}

Process::~Process( )
{
	Close( );
}

Process::Status Process::GetStatus( ) const
{
	if( !process )
		return Status::Unknown;

	return WaitForSingleObject( *process, 0 ) == WAIT_OBJECT_0 ? Status::Terminated : Status::Running;
}

bool Process::Close( )
{
	if( !process )
		return true;

	CloseInput( );

	if( WaitForSingleObject( *process, INFINITE ) == WAIT_OBJECT_0 )
	{
		DWORD code = 0;
		GetExitCodeProcess( *process, &code );
		exit_code = static_cast<int32_t>( code );
		process.reset( );
		return true;
	}

	return false;
}

void Process::Kill( )
{
	if( !process )
		return;

	TerminateProcess( *process, 0 );
	Close( );
}

void Process::CloseInput( )
{
	input_pipe.CloseWrite( );
}

Pipe &Process::Input( )
{
	return input_pipe;
}

Pipe &Process::Output( )
{
	return output_pipe;
}

Pipe &Process::Error( )
{
	return error_pipe;
}

int32_t Process::ExitCode( ) const
{
	return exit_code;
}

} // namespace MultiLibrary
