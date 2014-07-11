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

#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Common/IOStream.hpp>
#include <MultiLibrary/Common/Subscriber.hpp>
#include <string>
#include <set>
#include <memory>

namespace MultiLibrary
{

class FileInternal;

/*!
 \brief A class that represents a file.

 It provides data streaming capabilities.
 */
class MULTILIBRARY_FILESYSTEM_API File : public IOStream
{
public:
	File( FileInternal *file );

	virtual ~File( );

	bool IsValid( ) const;
	explicit operator bool( ) const;
	bool operator!( ) const;

	bool Close( );

	const std::string &GetPath( ) const;

	int64_t Tell( ) const;
	int64_t Size( ) const;
	bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET );

	bool Flush( );

	bool Errored( ) const;
	bool EndOfFile( ) const;

	size_t Read( void *value, size_t size );
	size_t Write( const void *value, size_t size );

	File &operator>>( bool &data );
	File &operator>>( int8_t &data );
	File &operator>>( uint8_t &data );
	File &operator>>( int16_t &data );
	File &operator>>( uint16_t &data );
	File &operator>>( int32_t &data );
	File &operator>>( uint32_t &data );
	File &operator>>( int64_t &data );
	File &operator>>( uint64_t &data );
	File &operator>>( float &data );
	File &operator>>( double &data );
	File &operator>>( char &data );
	File &operator>>( char *data ); // This is very dangerous and slow
	File &operator>>( std::string &data ); // This is slow and HDD heavy
	File &operator>>( wchar_t &data );
	File &operator>>( wchar_t *data ); // This is very dangerous and slow
	File &operator>>( std::wstring &data ); // This is slow and HDD heavy

	File &operator<<( const bool &data );
	File &operator<<( const int8_t &data );
	File &operator<<( const uint8_t &data );
	File &operator<<( const int16_t &data );
	File &operator<<( const uint16_t &data );
	File &operator<<( const int32_t &data );
	File &operator<<( const uint32_t &data );
	File &operator<<( const int64_t &data );
	File &operator<<( const uint64_t &data );
	File &operator<<( const float &data );
	File &operator<<( const double &data );
	File &operator<<( const char &data );
	File &operator<<( const char *data );
	File &operator<<( const std::string &data );
	File &operator<<( const wchar_t &data );
	File &operator<<( const wchar_t *data );
	File &operator<<( const std::wstring &data );

	void Subscribe( Subscriber *base );
	void Unsubscribe( Subscriber *base );

protected:
	std::set<Subscriber *> attached_subscribers;
	std::shared_ptr<FileInternal> file_internal;
};

} // namespace MultiLibrary