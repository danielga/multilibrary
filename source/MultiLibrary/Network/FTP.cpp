/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015, Daniel Almeida
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

#include <MultiLibrary/Network/FTP.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <fstream>
#include <sstream>
#include <iterator>

namespace MultiLibrary
{

class FTP::DataChannel : public NonCopyable
{
public:
	DataChannel( FTP &owner ) : ftp( owner ) { }

	FTP::Response Open( FTP::TransferMode mode )
	{
		FTP::Response response = ftp.SendCommand( "PASV" );
		if( response.IsOK( ) )
		{
			size_t begin = response.GetMessage( ).find_first_of( "0123456789" );
			if( begin != std::string::npos )
			{
				uint8_t data[6] = { 0, 0, 0, 0, 0, 0 };
				std::string str = response.GetMessage( ).substr( begin );
				size_t index = 0;
				for( size_t i = 0; i < sizeof( data ); ++i )
				{
					while( isdigit( str[index] ) )
					{
						data[i] = data[i] * 10 + ( str[index] - '0' );
						index++;
					}

					index++;
				}

				IPAddress address( data, data[4] * 256 + data[5] );

				if( !socket.Connect( address ) )
				{
					std::string modeStr;
					switch( mode )
					{
						case FTP::Binary:
							modeStr = "I";
							break;

						case FTP::Ascii:
							modeStr = "A";
							break;

						case FTP::Ebcdic:
							modeStr = "E";
							break;
					}

					response = ftp.SendCommand( "TYPE", modeStr );
				}
				else
				{
					response = FTP::Response( FTP::Response::ConnectionFailed );
				}
			}
		}

		return response;
	}

	void Send( const std::vector<char> &data )
	{
		if( !data.empty( ) )
			socket.Send( &data[0], static_cast<int>( data.size( ) ), 0 );

		socket.Close( );
	}

	void Receive( std::vector<char> &data )
	{
		data.clear( );
		char buffer[1024];
		int32_t received;
		while( !socket.Receive( buffer, sizeof( buffer ), 0, &received ) )
			std::copy( buffer, buffer + received, std::back_inserter( data ) );

		socket.Close( );
	}

private:
	FTP &ftp;
	SocketTCP socket;
};

FTP::Response::Response( Status code, const std::string &msg ) :
	status( code ),
	message( msg )
{ }

bool FTP::Response::IsOK( ) const
{
	return status < 400;
}

FTP::Response::Status FTP::Response::GetStatus( ) const
{
	return status;
}

const std::string &FTP::Response::GetMessage( ) const
{
	return message;
}

FTP::DirectoryResponse::DirectoryResponse( const Response &response ) :
	Response( response )
{
	if( IsOK( ) )
	{
		size_t begin = GetMessage( ).find( '"', 0 );
		size_t end = GetMessage( ).find( '"', begin + 1 );
		directory = GetMessage( ).substr( begin + 1, end - begin - 1 );
	}
}

const std::string &FTP::DirectoryResponse::GetDirectory( ) const
{
	return directory;
}

FTP::ListingResponse::ListingResponse( const Response &response, const std::vector<char> &data ) :
	Response( response )
{
	if( IsOK( ) )
	{
		std::string paths( data.begin( ), data.end( ) );
		size_t lastPos = 0;
		size_t pos = paths.find( "\r\n" );
		for( ; pos != std::string::npos; pos = paths.find( "\r\n", lastPos ) )
		{
			listing.push_back( paths.substr( lastPos, pos - lastPos ) );
			lastPos = pos + 2;
		}
	}
}

const std::vector<std::string> &FTP::ListingResponse::GetListing( ) const
{
	return listing;
}

FTP::~FTP( )
{
	Disconnect( );
}

FTP::Response FTP::Connect( const IPAddress &server, uint32_t timeout )
{
	connection_socket.Open( );
	connection_socket.SetTimeout( timeout );
	if( connection_socket.Connect( server ) )
		return Response( Response::ConnectionFailed );

	return GetResponse( );
}

FTP::Response FTP::Disconnect( )
{
	Response response = SendCommand( "QUIT" );
	if( response.IsOK( ) )
		connection_socket.Close( );

	return response;
}

FTP::Response FTP::Login( )
{
	return Login( "anonymous", "anonymous" );
}

FTP::Response FTP::Login( const std::string &name, const std::string &password )
{
	Response response = SendCommand( "USER", name );
	if( response.IsOK( ) )
		response = SendCommand( "PASS", password );

	return response;
}

FTP::Response FTP::KeepAlive( )
{
	return SendCommand( "NOOP" );
}

FTP::DirectoryResponse FTP::GetWorkingDirectory( )
{
	return DirectoryResponse( SendCommand( "PWD" ) );
}

FTP::Response FTP::ChangeDirectory( const std::string &directory )
{
	return SendCommand( "CWD", directory );
}

FTP::ListingResponse FTP::GetDirectoryListing( const std::string &directory )
{
	std::vector<char> directoryData;
	DataChannel data( *this );
	Response response = data.Open( Ascii );
	if( response.IsOK( ) )
	{
		response = SendCommand( "NLST", directory );
		if( response.IsOK( ) )
		{
			data.Receive( directoryData );
			response = GetResponse( );
		}
	}

	return ListingResponse( response, directoryData );
}

FTP::Response FTP::CreateDirectory( const std::string &name )
{
	return SendCommand( "MKD", name );
}

FTP::Response FTP::DeleteDirectory( const std::string &name )
{
	return SendCommand( "RMD", name );
}

FTP::Response FTP::RenameFile( const std::string &file, const std::string &newName )
{
	Response response = SendCommand( "RNFR", file );
	if( response.IsOK( ) )
		response = SendCommand( "RNTO", newName );

	return response;
}

FTP::Response FTP::DeleteFile( const std::string &name )
{
	return SendCommand( "DELE", name );
}

FTP::Response FTP::Download(
	const std::string &remoteFile,
	const std::string &localPath,
	FTP::TransferMode mode )
{
	DataChannel data( *this );
	Response response = data.Open( mode );
	if( response.IsOK( ) )
	{
		response = SendCommand( "RETR", remoteFile );
		if( response.IsOK( ) )
		{
			std::vector<char> fileData;
			data.Receive( fileData );

			response = GetResponse( );
			if( response.IsOK( ) )
			{
				std::string filename = remoteFile;
				size_t pos = filename.find_last_of( "/\\" );
				if( pos != std::string::npos )
					filename = filename.substr( pos + 1 );

				std::string path = localPath;
				if( !path.empty( ) && path[path.size( ) - 1] != '\\' && path[path.size( ) - 1] != '/' )
					path += "/";

				std::ofstream file( ( path + filename ).c_str( ), std::ios_base::binary );
				if( !file )
					return Response( Response::InvalidFile );

				if( !fileData.empty( ) )
					file.write( &fileData[0], static_cast<std::streamsize>( fileData.size( ) ) );
			}
		}
	}

	return response;
}

FTP::Response FTP::Upload( const std::string &localFile, const std::string &remotePath, FTP::TransferMode mode )
{
	std::ifstream file( localFile.c_str( ), std::ios_base::binary );
	if( !file )
		return Response( Response::InvalidFile );

	file.seekg( 0, std::ios::end );
	size_t length = static_cast<size_t>( file.tellg( ) );
	file.seekg( 0, std::ios::beg );
	std::vector<char> fileData( length );
	if( length > 0 )
		file.read( &fileData[0], length );

	std::string filename = localFile;
	std::string::size_type pos = filename.find_last_of( "/\\" );
	if( pos != std::string::npos )
		filename = filename.substr( pos + 1 );

	std::string path = remotePath;
	if( !path.empty( ) && ( path[path.size( ) - 1] != '\\' ) &&
		( path[path.size( ) - 1] != '/' ) )
		path += "/";

	DataChannel data( *this );
	Response response = data.Open( mode );
	if( response.IsOK( ) )
	{
		response = SendCommand( "STOR", path + filename );
		if( response.IsOK( ) )
		{
			data.Send( fileData );
			response = GetResponse( );
		}
	}

	return response;
}

FTP::Response FTP::SendCommand( const std::string &command, const std::string &parameter )
{
	std::string commandStr;
	if( parameter != "" )
		commandStr = command + " " + parameter + "\r\n";
	else
		commandStr = command + "\r\n";

	if( connection_socket.Send( commandStr.c_str( ), static_cast<int>( commandStr.length( ) ), 0 ) )
		return Response( Response::ConnectionClosed );

	return GetResponse( );
}

FTP::Response FTP::GetResponse( )
{
	uint32_t lastCode = 0;
	bool isInsideMultiline = false;
	std::string message;

	for( ;; )
	{
		char buffer[1024];
		int32_t length = 0;
		if( connection_socket.Receive( buffer, sizeof( buffer ), 0, &length ) )
			return Response( Response::ConnectionClosed );

		std::istringstream in( std::string( buffer, length ), std::ios_base::binary );
		while( in )
		{
			unsigned int code;
			if( in >> code )
			{
				char separator;
				in.get( separator );

				if( separator == '-' && !isInsideMultiline )
				{
					isInsideMultiline = true;

					if( lastCode == 0 )
						lastCode = code;

					std::getline( in, message );

					message.erase( message.length( ) - 1 );
					message = separator + message + "\n";
				}
				else
				{
					if( separator != '-' && ( code == lastCode || lastCode == 0 ) )
					{
						isInsideMultiline = false;

						std::string line;
						std::getline( in, line );

						line.erase( line.length( ) - 1 );

						if( code == lastCode )
						{
							std::ostringstream out;
							out << code << separator << line;
							message += out.str( );
						}
						else
						{
							message = separator + line;
						}

						return Response( static_cast<Response::Status>( code ), message );
					}
					else
					{
						std::string line;
						std::getline( in, line );

						if( !line.empty( ) )
						{
							line.erase( line.length( ) - 1 );

							std::ostringstream out;
							out << code << separator << line << "\n";
							message += out.str( );
						}
					}
				}
			}
			else if( lastCode != 0 )
			{
				in.clear( );

				std::string line;
				std::getline( in, line );

				if( !line.empty( ) )
				{
					line.erase( line.length( ) - 1 );

					message += line + "\n";
				}
			}
			else
			{
				return Response( Response::InvalidResponse );
			}
		}
	}
}

} // namespace MultiLibrary