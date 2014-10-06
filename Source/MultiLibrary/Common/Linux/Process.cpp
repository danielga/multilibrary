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
#include <system_error>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

namespace MultiLibrary
{

Process::Process( const std::string &path, const std::vector<std::string> &args ) :
	process( nullptr ),
	exit_code( 0 )
{
	char **argv = nullptr;
	if( !args.empty( ) )
	{
		argv = new char *[args.size( ) + 2];
		argv[0] = const_cast<char *>( path.c_str( ) );
		for( size_t i = 1; i < args.size( ) + 1; ++i )
			argv[i] = const_cast<char *>( args[i].c_str( ) );

		argv[args.size( )] = nullptr;
	}

	pid_t pid = fork( );
	if( pid == 0 && execv( path.c_str( ), argv ) )
	{
		delete [] argv;
		exit( EXIT_FAILURE );
	}

	delete [] argv;

	if( pid == -1 )
		throw std::system_error( errno, std::system_category( ), "failed to fork process" );

	process = reinterpret_cast<void *>( pid );
}

Process::~Process( )
{
	Close( );
}

Process::Status Process::GetStatus( ) const
{
	if( process == 0 )
		return Status::Unknown;

	int status = 0;
	if( waitpid( *reinterpret_cast<const pid_t *>( &process ), &status, WNOHANG ) )
		return WIFEXITED( status ) ? Status::Terminated : Status::Running;

	return Status::Unknown;
}

bool Process::Close( )
{
	if( process == 0 )
		return false;

	int status = 0;
	if( waitpid( *reinterpret_cast<pid_t *>( &process ), &status, 0 ) > 0 )
		return WIFEXITED( status );

	return false;
}

void Process::Kill( )
{
	if( process == 0 )
		return;

	kill( *reinterpret_cast<pid_t *>( &process ), 0 );
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