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

#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileInternal.hpp>
#include <cassert>
#include <cstring>

namespace MultiLibrary
{

File::File( FileInternal *file ) :
	file_internal( file )
{ }

File::~File( )
{
	std::set<Subscriber *>::iterator it, end = attached_subscribers.end( );
	for( it = attached_subscribers.begin( ); it != end; ++it )
		( *it )->ResetPublisher( );
}

bool File::IsValid( ) const
{
	return file_internal && file_internal->IsValid( );
}

File::operator bool( ) const
{
	return IsValid( );
}

bool File::operator!( ) const
{
	return !IsValid( );
}

bool File::Close( )
{
	if( file_internal )
	{
		file_internal.reset( );
		return true;
	}

	return false;
}

const std::string &File::GetPath( ) const
{
	if( file_internal )
		return file_internal->GetPath( );

	static const std::string invalid = "";
	return invalid;
}

int64_t File::Tell( ) const
{
	if( file_internal )
		return file_internal->Tell( );

	return 0;
}

int64_t File::Size( ) const
{
	if( file_internal )
		return file_internal->Size( );

	return 0;
}

bool File::Seek( int64_t pos, SeekMode mode )
{
	if( file_internal )
		return file_internal->Seek( pos, mode );

	return false;
}

bool File::Flush( )
{
	if( file_internal )
		return file_internal->Flush( );

	return false;
}

bool File::Errored( ) const
{
	if( file_internal )
		return file_internal->Errored( );

	return false;
}

bool File::EndOfFile( ) const
{
	if( file_internal )
		return file_internal->EndOfFile( );

	return false;
}

size_t File::Read( void *data, size_t size )
{
	assert( data != nullptr && size != 0 );

	if( file_internal )
		return file_internal->Read( data, size );

	return 0;
}

size_t File::Write( const void *data, size_t size )
{
	assert( data != nullptr && size != 0 );

	if( file_internal )
		return file_internal->Write( data, size );

	return 0;
}

File &File::operator>>( bool &data )
{
	bool value;
	if( Read( &value, sizeof( bool ) ) == sizeof( bool ) )
		data = value;

	return *this;
}

File &File::operator>>( int8_t &data )
{
	int8_t value;
	if( Read( &value, sizeof( int8_t ) ) == sizeof( int8_t ) )
		data = value;

	return *this;
}

File &File::operator>>( uint8_t &data )
{
	uint8_t value;
	if( Read( &value, sizeof( uint8_t ) ) == sizeof( uint8_t ) )
		data = value;

	return *this;
}

File &File::operator>>( int16_t &data )
{
	int16_t value;
	if( Read( &value, sizeof( int16_t ) ) == sizeof( int16_t ) )
		data = value;

	return *this;
}

File &File::operator>>( uint16_t &data )
{
	uint16_t value;
	if( Read( &value, sizeof( uint16_t ) ) == sizeof( uint16_t ) )
		data = value;

	return *this;
}

File &File::operator>>( int32_t &data )
{
	int32_t value;
	if( Read( &value, sizeof( int32_t ) ) == sizeof( int32_t ) )
		data = value;

	return *this;
}

File &File::operator>>( uint32_t &data )
{
	uint32_t value;
	if( Read( &value, sizeof( uint32_t ) ) == sizeof( uint32_t ) )
		data = value;

	return *this;
}

File &File::operator>>( int64_t &data )
{
	int64_t value;
	if( Read( &value, sizeof( int64_t ) ) == sizeof( int64_t ) )
		data = value;

	return *this;
}

File &File::operator>>( uint64_t &data )
{
	uint64_t value;
	if( Read( &value, sizeof( uint64_t ) ) == sizeof( uint64_t ) )
		data = value;

	return *this;
}

File &File::operator>>( float &data )
{
	float value;
	if( Read( &value, sizeof( float ) ) == sizeof( float ) )
		data = value;

	return *this;
}

File &File::operator>>( double &data )
{
	double value;
	if( Read( &value, sizeof( double ) ) == sizeof( double ) )
		data = value;

	return *this;
}

File &File::operator>>( char &data )
{
	char value;
	if( Read( &value, sizeof( char ) ) == sizeof( char ) )
		data = value;

	return *this;
}

File &File::operator>>( char *data )
{
	assert( data != nullptr );

	char ch = 0;
	size_t offset = 0;
	while( *this >> ch )
	{
		data[offset] = ch;

		if( ch == 0 )
			break;

		++offset;
	}

	return *this;
}

File &File::operator>>( std::string &data )
{
	char ch = 0;
	while( *this >> ch && ch != 0 )
		data += ch;

	return *this;
}

File &File::operator>>( wchar_t &data )
{
	wchar_t value;
	if( Read( &value, sizeof( wchar_t ) ) == sizeof( wchar_t ) )
		data = value;

	return *this;
}

File &File::operator>>( wchar_t *data )
{
	assert( data != nullptr );

	wchar_t ch = 0;
	size_t offset = 0;
	while( *this >> ch )
	{
		data[offset] = ch;

		if( ch == 0 )
			break;

		++offset;
	}

	return *this;
}

File &File::operator>>( std::wstring &data )
{
	wchar_t ch = 0;
	while( *this >> ch && ch != 0 )
		data += ch;

	return *this;
}

File &File::operator<<( const bool &data )
{
	Write( &data, sizeof( bool ) );
	return *this;
}

File &File::operator<<( const int8_t &data )
{
	Write( &data, sizeof( int8_t ) );
	return *this;
}

File &File::operator<<( const uint8_t &data )
{
	Write( &data, sizeof( uint8_t ) );
	return *this;
}

File &File::operator<<( const int16_t &data )
{
	Write( &data, sizeof( int16_t ) );
	return *this;
}

File &File::operator<<( const uint16_t &data )
{
	Write( &data, sizeof( uint16_t ) );
	return *this;
}

File &File::operator<<( const int32_t &data )
{
	Write( &data, sizeof( int32_t ) );
	return *this;
}

File &File::operator<<( const uint32_t &data )
{
	Write( &data, sizeof( uint32_t ) );
	return *this;
}

File &File::operator<<( const float &data )
{
	Write( &data, sizeof( float ) );
	return *this;
}

File &File::operator<<( const double &data )
{
	Write( &data, sizeof( double ) );
	return *this;
}

File &File::operator<<( const char &data )
{
	Write( &data, sizeof( char ) );
	return *this;
}

File &File::operator<<( const char *data )
{
	Write( data, ( std::strlen( data ) + 1 ) * sizeof( char ) );
	return *this;
}

File &File::operator<<( const std::string &data )
{
	Write( data.c_str( ), ( data.length( ) + 1 ) * sizeof( char ) );
	return *this;
}

File &File::operator<<( const wchar_t &data )
{
	Write( &data, sizeof( wchar_t ) );
	return *this;
}

File &File::operator<<( const wchar_t *data )
{
	Write( data, ( std::wcslen( data ) + 1 ) * sizeof( wchar_t ) );
	return *this;
}

File &File::operator<<( const std::wstring &data )
{
	Write( data.c_str( ), ( data.length( ) + 1 ) * sizeof( wchar_t ) );
	return *this;
}

void File::Subscribe( Subscriber *base )
{
	attached_subscribers.insert( base );
}

void File::Unsubscribe( Subscriber *base )
{
	attached_subscribers.erase( base );
}

} // namespace MultiLibrary