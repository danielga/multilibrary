#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Common/Process.hpp>
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
		read_handle( NULL ),
		write_handle( NULL )
	{ }

	~Pipe( )
	{
		Close( );
	}

	void Open( )
	{
		if( read_handle != NULL || write_handle != NULL )
			return;

		SECURITY_ATTRIBUTES sa;
		sa.nLength = static_cast<DWORD>( sizeof( SECURITY_ATTRIBUTES ) );
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;
		if( CreatePipe( &read_handle, &write_handle, &sa, 0 ) == FALSE )
			throw std::runtime_error( "unable to create pipe" );
	}

	void CloseRead( )
	{
		if( read_handle != NULL )
		{
			if( CloseHandle( read_handle ) == FALSE )
				throw std::runtime_error( "unable to close pipe read handle" );

			read_handle = NULL;
		}
	}

	void CloseWrite( )
	{
		if( write_handle != NULL )
		{
			if( CloseHandle( write_handle ) == FALSE )
				throw std::runtime_error( "unable to close pipe write handle" );

			write_handle = NULL;
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
		if( pipe.Read( ) != NULL && gptr( ) == egptr( ) )
		{
			DWORD size = 0;
			if( PeekNamedPipe( pipe.Read( ), NULL, 0, NULL, &size, NULL ) == FALSE || size == 0 )
				return traits_type::eof( );

			if( ReadFile( pipe.Read( ), stream_buffer, STREAM_BUFFER_SIZE, &size, NULL ) == FALSE || size == 0 )
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
		if( pipe.Write( ) != NULL && pbase( ) != pptr( ) )
		{
			DWORD size = static_cast<DWORD>( pptr( ) - pbase( ) ), written = 0;
			if( WriteFile( pipe.Write( ), pbase( ), size, &written, NULL ) == TRUE && written == size )
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

struct Data : public NonCopyable
{
	Data( ) :
		handle( NULL ),
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

	Pipe input_pipe;
	Pipe output_pipe;
	Pipe error_pipe;

	PipeWriteStream input_streambuf;
	PipeReadStream output_streambuf;
	PipeReadStream error_streambuf;

	std::ostream input_stream;
	std::istream output_stream;
	std::istream error_stream;
};

} // namespace Internal

Process::Process( ) :
	process( NULL ),
	exit_code( 0 )
{ }

Process::~Process( )
{
	Close( );
}

bool Process::Start( const std::string &path, const std::string &args )
{
	if( process != NULL )
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

	process = new Internal::Data;

	STARTUPINFO startup;
	memset( &startup, 0, sizeof( STARTUPINFO ) );
	startup.cb = sizeof( STARTUPINFO );
	startup.dwFlags = STARTF_USESTDHANDLES;
	startup.hStdInput = process->input_pipe.Read( );
	startup.hStdOutput = process->output_pipe.Write( );
	startup.hStdError = process->error_pipe.Write( );

	PROCESS_INFORMATION info;

	if( CreateProcess( NULL, &cmdline[0], NULL, NULL, TRUE, 0, NULL, NULL, &startup, &info ) == TRUE )
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
	process = NULL;
	return false;
}

bool Process::Start( const std::string &path, const std::vector<std::string> &args )
{
	if( process != NULL )
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

bool Process::Wait( const Time &timeout )
{
	if( process == NULL )
		return false;

	return WaitForSingleObject( process->handle, static_cast<DWORD>( timeout.Milliseconds( ) ) ) == WAIT_OBJECT_0;
}

bool Process::Close( )
{
	if( process == NULL )
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
		process = NULL;
		return true;
	}

	return false;
}

void Process::Kill( )
{
	if( process == NULL )
		return;

	if( TerminateProcess( process->handle, 0 ) == FALSE || !Close( ) )
		throw std::runtime_error( "unable to terminate process" );
}

void Process::CloseInput( )
{
	if( process != NULL && process->input_pipe.Write( ) != NULL )
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