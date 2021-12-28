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

#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileInternal.hpp>
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>

namespace MultiLibrary
{

File::File( const std::shared_ptr<FileInternal> &file ) :
	file_internal( file )
{ }

File::File( std::shared_ptr<FileInternal> &&file )
{
	file_internal.swap( file );
}

File::~File( )
{
	Close( );
}

bool File::IsValid( ) const
{
	return file_internal && file_internal->IsValid( );
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

size_t File::Tell( ) const
{
	if( file_internal )
	{
		int64_t offset = file_internal->Tell( );
		return offset > 0 ? static_cast<size_t>( offset ) : 0;
	}

	return 0;
}

size_t File::Size( ) const
{
	if( file_internal )
	{
		int64_t size = file_internal->Size( );
		return size > 0 ? static_cast<size_t>( size ) : 0;
	}

	return 0;
}

bool File::Seek( size_t pos )
{
	if( file_internal )
		return file_internal->Seek( pos, SeekMode::Set );

	return false;
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
	if( file_internal )
		return file_internal->Read( data, size );

	return 0;
}

size_t File::Write( const void *data, size_t size )
{
	if( file_internal )
		return file_internal->Write( data, size );

	return 0;
}

InputStream &File::operator>>( bool &data )
{
	char value[6] = { 0 };
	if( file_internal && Read( value, sizeof( *value ) * 5 ) == sizeof( *value ) * 5 )
	{
		if( strcmp( value, "false" ) == 0 )
		{
			data = false;
		}
		else if( strncmp( value, "true", 4 ) == 0 )
		{
			Seek( -1, SeekMode::Cur );
			data = true;
		}
		else
		{
			Seek( -5, SeekMode::Cur );
		}
	}

	return *this;
}

InputStream &File::operator>>( int8_t &data )
{
	int8_t value;
	if( file_internal && file_internal->Scan( "%hhi", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( uint8_t &data )
{
	uint8_t value;
	if( file_internal && file_internal->Scan( "%hhu", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( int16_t &data )
{
	int16_t value;
	if( file_internal && file_internal->Scan( "%hi", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( uint16_t &data )
{
	uint16_t value;
	if( file_internal && file_internal->Scan( "%hu", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( int32_t &data )
{
	int32_t value;
	if( file_internal && file_internal->Scan( "%i", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( uint32_t &data )
{
	uint32_t value;
	if( file_internal && file_internal->Scan( "%u", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( int64_t &data )
{
	int64_t value;
	if( file_internal && file_internal->Scan( "%lli", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( uint64_t &data )
{
	uint64_t value;
	if( file_internal && file_internal->Scan( "%llu", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( float &data )
{
	float value;
	if( file_internal && file_internal->Scan( "%f", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( double &data )
{
	double value;
	if( file_internal && file_internal->Scan( "%lf", &value ) == 1 )
		data = value;

	return *this;
}

InputStream &File::operator>>( char &data )
{
	char value;
	if( Read( &value, sizeof( char ) ) == sizeof( char ) )
		data = value;

	return *this;
}

InputStream &File::operator>>( char *data )
{
	assert( data != nullptr );

	char ch = '\0';
	size_t offset = 0;
	while( Read( &ch, sizeof( ch ) ) == sizeof( ch ) )
	{
		data[offset] = ch;

		if( ch == '\r' || ch == '\n' )
			break;

		++offset;
	}

	return *this;
}

InputStream &File::operator>>( std::string &data )
{
	char ch = '\0';
	while( Read( &ch, sizeof( ch ) ) == sizeof( ch ) && ch != '\r' && ch != '\n' )
		data += ch;

	return *this;
}

InputStream &File::operator>>( wchar_t &data )
{
	wchar_t value;
	if( Read( &value, sizeof( value ) ) == sizeof( value ) )
		data = value;

	return *this;
}

InputStream &File::operator>>( wchar_t *data )
{
	assert( data != nullptr );

	wchar_t ch = L'\0';
	size_t offset = 0;
	while( Read( &ch, sizeof( ch ) ) == sizeof( ch ) )
	{
		data[offset] = ch;

		if( ch == L'\r' || ch == L'\n' )
			break;

		++offset;
	}

	return *this;
}

InputStream &File::operator>>( std::wstring &data )
{
	wchar_t ch = L'\0';
	while( Read( &ch, sizeof( ch ) ) == sizeof( ch ) && ch != L'\r' && ch != L'\n' )
		data += ch;

	return *this;
}

OutputStream &File::operator<<( const bool &data )
{
	if( file_internal )
		file_internal->Print( data ? "true" : "false" );

	return *this;
}

OutputStream &File::operator<<( const int8_t &data )
{
	if( file_internal )
		file_internal->Print( "%hhi", data );

	return *this;
}

OutputStream &File::operator<<( const uint8_t &data )
{
	if( file_internal )
		file_internal->Print( "%hhu", data );

	return *this;
}

OutputStream &File::operator<<( const int16_t &data )
{
	if( file_internal )
		file_internal->Print( "%hi", data );

	return *this;
}

OutputStream &File::operator<<( const uint16_t &data )
{
	if( file_internal )
		file_internal->Print( "%hu", data );

	return *this;
}

OutputStream &File::operator<<( const int32_t &data )
{
	if( file_internal )
		file_internal->Print( "%i", data );

	return *this;
}

OutputStream &File::operator<<( const uint32_t &data )
{
	if( file_internal )
		file_internal->Print( "%u", data );

	return *this;
}

OutputStream &File::operator<<( const int64_t &data )
{
	if( file_internal )
		file_internal->Print( "%lli", data );

	return *this;
}

OutputStream &File::operator<<( const uint64_t &data )
{
	if( file_internal )
		file_internal->Print( "%llu", data );

	return *this;
}

OutputStream &File::operator<<( const float &data )
{
	if( file_internal )
		file_internal->Print( "%f", data );

	return *this;
}

OutputStream &File::operator<<( const double &data )
{
	if( file_internal )
		file_internal->Print( "%lf", data );

	return *this;
}

OutputStream &File::operator<<( const char &data )
{
	Write( &data, sizeof( char ) );
	return *this;
}

OutputStream &File::operator<<( const char *data )
{
	assert( data != nullptr );

	Write( data, std::strlen( data ) * sizeof( char ) );
	return *this;
}

OutputStream &File::operator<<( const std::string &data )
{
	Write( data.c_str( ), data.length( ) * sizeof( char ) );
	return *this;
}

OutputStream &File::operator<<( const wchar_t &data )
{
	Write( &data, sizeof( wchar_t ) );
	return *this;
}

OutputStream &File::operator<<( const wchar_t *data )
{
	assert( data != nullptr );

	Write( data, std::wcslen( data ) * sizeof( wchar_t ) );
	return *this;
}

OutputStream &File::operator<<( const std::wstring &data )
{
	Write( data.c_str( ), data.length( ) * sizeof( wchar_t ) );
	return *this;
}

} // namespace MultiLibrary
