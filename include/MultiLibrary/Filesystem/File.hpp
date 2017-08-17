/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2017, Daniel Almeida
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
	File( const std::shared_ptr<FileInternal> &file );
	File( std::shared_ptr<FileInternal> &&file );
	virtual ~File( );

	bool IsValid( ) const;

	bool Close( );

	const std::string &GetPath( ) const;

	int64_t Tell( ) const;
	int64_t Size( ) const;
	bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET );

	bool Flush( );

	bool Errored( ) const;
	bool EndOfFile( ) const;

	/*!
	 \brief Reads the specified amount of bytes into the provided buffer.

	 \param data Buffer to store the data.
	 \param size Size of the buffer.

	 \return Amount of read bytes.
	 */
	size_t Read( void *data, size_t size );

	/*!
	 \brief Writes the specified amount of bytes from the provided buffer.

	 \param data Data to write.
	 \param size Size of the data.

	 \return Amount of written bytes.
	 */
	size_t Write( const void *data, size_t size );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.
	 */
	InputStream &operator>>( bool &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( int8_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( uint8_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( int16_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( uint16_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( int32_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( uint32_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( int64_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( uint64_t &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( float &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( double &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( char &data );

	/*!
	 \brief Read data from the buffer into an array.

	 \param data Where to store the data.

	 \return This object.

	 \overload

	 \deprecated This function is unsafe since the array size is unknown.
	 */
	InputStream &operator>>( char *data );

	/*!
	 \brief Read data from the buffer into an object.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( std::string &data );

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( wchar_t &data );

	/*!
	 \brief Read data from the buffer into an array.

	 \param data Where to store the data.

	 \return This object.

	 \overload

	 \deprecated This function is unsafe since the array size is unknown.
	 */
	InputStream &operator>>( wchar_t *data );

	/*!
	 \brief Read data from the buffer into an object.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( std::wstring &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const bool &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const int8_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const uint8_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const int16_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const uint16_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const int32_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const uint32_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const int64_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const uint64_t &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const float &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const double &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const char &data );

	/*!
	 \brief Write data into the buffer from an array.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const char *data );

	/*!
	 \brief Write data into the buffer from an object.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const std::string &data );

	/*!
	 \brief Write data into the buffer from a variable.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const wchar_t &data );

	/*!
	 \brief Write data into the buffer from an array.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const wchar_t *data );

	/*!
	 \brief Write data into the buffer from an object.

	 \param data Data to write.

	 \return This object.

	 \overload
	 */
	OutputStream &operator<<( const std::wstring &data );

protected:
	std::shared_ptr<FileInternal> file_internal;
};

} // namespace MultiLibrary
