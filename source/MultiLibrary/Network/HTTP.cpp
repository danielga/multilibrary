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

#include <MultiLibrary/Network/HTTP.hpp>
#include <MultiLibrary/Network/SocketTCP.hpp>
#include <cctype>
#include <sstream>
#include <iterator>

namespace MultiLibrary
{
HTTP::Request::Request( const std::string &uri, Method method, const std::string &body ) :
	uri( uri ),
	body( body ),
	method( method ),
	minorVersion( 0 ),
	majorVersion( 1 )
{ }

void HTTP::Request::SetURI( const std::string &u )
{
	uri = u;
}

void HTTP::Request::SetMethod( HTTP::Request::Method m )
{
	method = m;
}

void HTTP::Request::SetHTTPVersion( uint32_t major, uint32_t minor )
{
	majorVersion = major;
	minorVersion = minor;
}

void HTTP::Request::SetBody( const std::string &b )
{
	body = b;
}

void HTTP::Request::SetField( const std::string &key, const std::string &value )
{
	fields[key] = value;
}

bool HTTP::Request::HasField( const std::string &key ) const
{
	return fields.find( key ) != fields.end( );
}

std::string HTTP::Request::BuildRequest( ) const
{
	std::ostringstream out;

	switch( method )
	{
		default:
		case Get:
			out << "GET ";
			break;

		case Post:
			out << "POST ";
			break;

		case Head:
			out << "HEAD ";
			break;
	}

	out << uri << " HTTP/" << majorVersion << "." << minorVersion << "\r\n";

	std::map<std::string, std::string>::const_iterator i;
	for( i = fields.begin( ); i != fields.end( ); ++i )
	{
		out << i->first << ": " << i->second << "\r\n";
	}

	out << "\r\n" << body;
	return out.str( );
}

HTTP::Response::Response( ) :
	status( ConnectionFailed ),
	minorVersion( 0 ),
	majorVersion( 0 )
{ }

HTTP::Response::Status HTTP::Response::GetStatus( ) const
{
	return status;
}

uint32_t HTTP::Response::GetHTTPMajorVersion( ) const
{
	return majorVersion;
}

uint32_t HTTP::Response::GetHTTPMinorVersion( ) const
{
	return minorVersion;
}

const std::string &HTTP::Response::GetBody( ) const
{
	return body;
}

const std::string &HTTP::Response::GetField( const std::string &key ) const
{
	std::map<std::string, std::string>::const_iterator it = fields.find( key );
	if( it != fields.end( ) )
	{
		return it->second;
	}
	else
	{
		static const std::string empty = "";
		return empty;
	}
}

void HTTP::Response::ParseResponse( const std::string &data )
{
	std::istringstream in( data );
	std::string version;
	if( in >> version )
	{
		if( version.size( ) >= 8 && version[6] == '.' && version.substr( 0, 5 ) == "HTTP/" && std::isdigit( version[5] ) && std::isdigit( version[7] ) )
		{
			majorVersion = version[5] - '0';
			minorVersion = version[7] - '0';
		}
		else
		{
			status = InvalidResponse;
			return;
		}
	}

	int stat;
	if( in >> stat )
	{
		status = static_cast<Status>( stat );
	}
	else
	{
		status = InvalidResponse;
		return;
	}

	in.ignore( 10000, '\n' );

	std::string line;
	while( std::getline( in, line ) && line.size( ) > 2 )
	{
		std::string::size_type pos = line.find( ": " );
		if( pos != std::string::npos )
		{
			std::string field = line.substr( 0, pos );
			std::string value = line.substr( pos + 2 );

			if( !value.empty( ) && *value.rbegin( ) == '\r' )
				value.erase( value.size( ) - 1 );

			fields[field] = value;
		}
	}

	body.clear( );
	std::copy( std::istreambuf_iterator<char>( in ), std::istreambuf_iterator<char>( ), std::back_inserter( body ) );
}

HTTP::HTTP( ) { }

HTTP::HTTP( const std::string &host, uint16_t port )
{
	SetHost( host, port );
}

HTTP::~HTTP( ) { }

void HTTP::SetHost( const std::string &host, uint16_t port )
{
	std::string protocol = host.substr( 0, 8 );
	if( protocol.substr( 0, 7 ) == "http://" )
	{
		host_name = host.substr( 7 );
		port = port != 0 ? port : 80;
	}
	else if( protocol == "https://" )
	{
		host_name = host.substr( 8 );
		port = port != 0 ? port : 443;
	}
	else
	{
		host_name = host;
		port = port != 0 ? port : 80;
	}

	if( !host_name.empty( ) && ( *host_name.rbegin( ) == '/' ) )
		host_name.erase( host_name.size( ) - 1 );

	host_address = IPAddress( host_name, port );
}

HTTP::Response HTTP::SendRequest( const HTTP::Request &request, uint32_t timeout )
{
	Request req( request );
	if( !req.HasField( "From" ) )
		req.SetField( "From", "someguy" );

	if( !req.HasField( "User-Agent" ) )
		req.SetField( "User-Agent", "multilibrarynetwork" );

	if( !req.HasField( "Host" ) )
		req.SetField( "Host", host_name );

	if( !req.HasField( "Content-Length" ) )
	{
		std::ostringstream out;
		out << req.body.size( );
		req.SetField( "Content-Length", out.str( ) );
	}

	if( ( req.method == Request::Post ) && !req.HasField( "Content-Type" ) )
		req.SetField( "Content-Type", "application/x-www-form-urlencoded" );

	if( ( req.majorVersion * 10 + req.minorVersion >= 11 ) && !req.HasField( "Connection" ) )
		req.SetField( "Connection", "close" );

	Response response;
	SocketTCP connection;
	connection.SetBlocking( true );
	connection.SetTimeout( timeout );
	if( !connection.Connect( host_address ) )
	{
		std::string reqString = req.BuildRequest( );
		if( !reqString.empty( ) &&
			!connection.Send( reqString.c_str( ), static_cast<int>( reqString.size( ) ), 0 ) )
		{
			std::string received;
			int size = 0;
			char buffer[1024];
			while( !connection.Receive( buffer, sizeof( buffer ), 0, &size ) )
			{
				received.append( buffer, buffer + size );
				if( size < static_cast<int>( sizeof( buffer ) ) )
				{
					break;
				}
			}

			response.ParseResponse( received );
		}

		connection.Shutdown( 2 );
		connection.Close( );
	}

	return response;
}

} // namespace MultiLibrary