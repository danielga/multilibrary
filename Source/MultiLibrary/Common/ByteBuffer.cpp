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

#include <MultiLibrary/Common/ByteBuffer.hpp>
#include <MultiLibrary/Common/Subscriber.hpp>
#include <stdexcept>
#include <cassert>
#include <cstring>

namespace MultiLibrary
{

ByteBuffer::ByteBuffer( ) :
	end_of_file( true ),
	buffer_offset( 0 )
{ }

ByteBuffer::ByteBuffer( size_t size ) :
	end_of_file( true ),
	buffer_offset( 0 )
{
	Resize( size );
}

ByteBuffer::ByteBuffer( const uint8_t *copy_buffer, size_t size ) :
	end_of_file( true ),
	buffer_offset( 0 )
{
	Assign( copy_buffer, size );
}

ByteBuffer::~ByteBuffer( )
{
	std::set<Subscriber *>::iterator it, end = attached_subscribers.end( );
	for( it = attached_subscribers.begin( ); it != end; ++it )
		( *it )->ResetPublisher( );
}

bool ByteBuffer::IsValid( ) const
{
	return !EndOfFile( );
}

ByteBuffer::operator bool( ) const
{
	return IsValid( );
}

bool ByteBuffer::operator!( ) const
{
	return !IsValid( );
}

int64_t ByteBuffer::Tell( ) const
{
	return static_cast<int64_t>( buffer_offset );
}

int64_t ByteBuffer::Size( ) const
{
	return static_cast<int64_t>( buffer_internal.size( ) );
}

size_t ByteBuffer::Capacity( ) const
{
	return buffer_internal.capacity( );
}

bool ByteBuffer::Seek( int64_t position, SeekMode mode )
{
	assert( mode != SEEKMODE_SET || ( mode == SEEKMODE_SET && position >= 0 ) );
	assert( mode != SEEKMODE_CUR || ( mode == SEEKMODE_CUR && Tell( ) + position >= 0 ) );
	assert( mode != SEEKMODE_END || ( mode == SEEKMODE_END && Size( ) + position >= 0 ) );

	int64_t temp;
	switch( mode )
	{
	case SEEKMODE_SET:
		buffer_offset = static_cast<size_t>( position > 0 ? position : 0 );
		break;

	case SEEKMODE_CUR:
		temp = Tell( ) + position;
		buffer_offset = static_cast<size_t>( temp > 0 ? temp : 0 );
		break;

	case SEEKMODE_END:
		temp = Size( ) + position;
		buffer_offset = static_cast<size_t>( temp > 0 ? temp : 0 );
		break;

	default:
		return false;
	}

	end_of_file = false;
	return true;
}

bool ByteBuffer::EndOfFile( ) const
{
	return end_of_file;
}

uint8_t *ByteBuffer::GetBuffer( )
{
	return &buffer_internal[0];
}

const uint8_t *ByteBuffer::GetBuffer( ) const
{
	return &buffer_internal[0];
}

void ByteBuffer::Clear( )
{
	buffer_internal.clear( );
	buffer_offset = 0;
	end_of_file = false;
}

void ByteBuffer::Reserve( size_t capacity )
{
	buffer_internal.reserve( capacity );
}

void ByteBuffer::Resize( size_t size )
{
	buffer_internal.resize( size );
}

void ByteBuffer::ShrinkToFit( )
{
	std::vector<uint8_t>( buffer_internal ).swap( buffer_internal );
}

void ByteBuffer::Assign( const uint8_t *copy_buffer, size_t size )
{
	assert( copy_buffer != nullptr && size != 0 );

	buffer_internal.assign( copy_buffer, copy_buffer + size );
	buffer_offset = 0;
	end_of_file = false;
}

size_t ByteBuffer::Read( void *value, size_t size )
{
	assert( value != nullptr && size != 0 );

	if( buffer_offset >= buffer_internal.size( ) )
	{
		end_of_file = true;
		return 0;
	}

	size_t clamped = buffer_internal.size( ) - buffer_offset;
	if( clamped > size )
		clamped = size;
	std::memcpy( value, &buffer_internal[buffer_offset], clamped );
	buffer_offset += clamped;
	if( clamped < size )
		end_of_file = true;

	return clamped;
}

size_t ByteBuffer::Write( const void *value, size_t size )
{
	assert( value != nullptr && size != 0 );

	if( buffer_internal.size( ) < buffer_offset + size )
		Resize( buffer_offset + size );

	std::memcpy( &buffer_internal[buffer_offset], value, size );
	buffer_offset += size;
	return size;
}

ByteBuffer &ByteBuffer::operator>>( bool &data )
{
	bool value;
	if( Read( &value, sizeof( bool ) ) == sizeof( bool ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( int8_t &data )
{
	int8_t value;
	if( Read( &value, sizeof( int8_t ) ) == sizeof( int8_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( uint8_t &data )
{
	uint8_t value;
	if( Read( &value, sizeof( uint8_t ) ) == sizeof( uint8_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( int16_t &data )
{
	int16_t value;
	if( Read( &value, sizeof( int16_t ) ) == sizeof( int16_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( uint16_t &data )
{
	uint16_t value;
	if( Read( &value, sizeof( uint16_t ) ) == sizeof( uint16_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( int32_t &data )
{
	int32_t value;
	if( Read( &value, sizeof( int32_t ) ) == sizeof( int32_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( uint32_t &data )
{
	uint32_t value;
	if( Read( &value, sizeof( uint32_t ) ) == sizeof( uint32_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( int64_t &data )
{
	int64_t value;
	if( Read( &value, sizeof( int64_t ) ) == sizeof( int64_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( uint64_t &data )
{
	uint64_t value;
	if( Read( &value, sizeof( uint64_t ) ) == sizeof( uint64_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( float &data )
{
	float value;
	if( Read( &value, sizeof( float ) ) == sizeof( float ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( double &data )
{
	double value;
	if( Read( &value, sizeof( double ) ) == sizeof( double ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( char &data )
{
	char value;
	if( Read( &value, sizeof( char ) ) == sizeof( char ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( char *data )
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

ByteBuffer &ByteBuffer::operator>>( std::string &data )
{
	char ch = 0;
	while( *this >> ch && ch != 0 )
		data += ch;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( wchar_t &data )
{
	wchar_t value;
	if( Read( &value, sizeof( wchar_t ) ) == sizeof( wchar_t ) )
		data = value;

	return *this;
}

ByteBuffer &ByteBuffer::operator>>( wchar_t *data )
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

ByteBuffer &ByteBuffer::operator>>( std::wstring &data )
{
	wchar_t ch = 0;
	while( *this >> ch && ch != 0 )
		data += ch;

	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const bool &data )
{
	Write( &data, sizeof( bool ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const int8_t &data )
{
	Write( &data, sizeof( int8_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const uint8_t &data )
{
	Write( &data, sizeof( uint8_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const int16_t &data )
{
	Write( &data, sizeof( int16_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const uint16_t &data )
{
	Write( &data, sizeof( uint16_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const int32_t &data )
{
	Write( &data, sizeof( int32_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const uint32_t &data )
{
	Write( &data, sizeof( uint32_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const int64_t &data )
{
	Write( &data, sizeof( int64_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const uint64_t &data )
{
	Write( &data, sizeof( uint64_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const float &data )
{
	Write( &data, sizeof( float ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const double &data )
{
	Write( &data, sizeof( double ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const char &data )
{
	Write( &data, sizeof( char ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const char *data )
{
	assert( data != nullptr );

	Write( data, ( std::strlen( data ) + 1 ) * sizeof( char ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const std::string &data )
{
	Write( data.c_str( ), ( data.length( ) + 1 ) * sizeof( char ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const wchar_t &data )
{
	Write( &data, sizeof( wchar_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const wchar_t *data )
{
	assert( data != nullptr );

	Write( data, ( std::wcslen( data ) + 1 ) * sizeof( wchar_t ) );
	return *this;
}

ByteBuffer &ByteBuffer::operator<<( const std::wstring &data )
{
	Write( data.c_str( ), ( data.length( ) + 1 ) * sizeof( wchar_t ) );
	return *this;
}

void ByteBuffer::Subscribe( Subscriber *base )
{
	attached_subscribers.insert( base );
}

void ByteBuffer::Unsubscribe( Subscriber *base )
{
	attached_subscribers.erase( base );
}

} // namespace MultiLibrary