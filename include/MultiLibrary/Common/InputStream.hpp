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

#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/Stream.hpp>
#include <string>

namespace MultiLibrary
{

/*!
 \brief An abstract class for objects that can act as input data streams.
 */
class MULTILIBRARY_COMMON_API InputStream : public Stream
{
public:
	/*!
	 \brief Reads the specified amount of bytes into the provided buffer.

	 \param data Buffer to store the data.
	 \param size Size of the buffer.

	 \return Amount of read bytes.
	 */
	virtual size_t Read( void *data, size_t size ) = 0;

	/*!
	 \brief Read data from the buffer into a variable.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	template<typename Type>
	InputStream &operator>>( Type &data )
	{
		Type value;
		if( Read( &value, sizeof( Type ) ) == sizeof( Type ) )
			data = value;

		return *this;
	}

	/*!
	\brief Read data from the buffer into an array.

	\param data Where to store the data.

	\return This object.

	\overload
	*/
	template<size_t Length>
	InputStream &operator>>( char ( &data )[Length] )
	{
		char ch = '\0';
		size_t offset = 0;
		while( offset < Length && Read( &ch, sizeof( ch ) ) == sizeof( ch ) )
		{
			data[offset] = ch;

			if( ch == '\0' || ch == '\n' )
				return *this;

			++offset;
		}

		if( offset >= Length )
			offset = Length - 1;

		data[offset] = '\0';
		return *this;
	}

	/*!
	 \brief Read data from the buffer into an object.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( std::string &data );

	/*!
	\brief Read data from the buffer into an array.

	\param data Where to store the data.

	\return This object.

	\overload
	*/
	template<size_t Length>
	InputStream &operator>>( wchar_t ( &data )[Length] )
	{
		wchar_t ch = L'\0';
		size_t offset = 0;
		while( offset < Length && Read( &ch, sizeof( ch ) ) == sizeof( ch ) )
		{
			data[offset] = ch;

			if( ch == L'\0' || ch == L'\n' )
				return *this;

			++offset;
		}

		if( offset >= Length )
			offset = Length - 1;

		data[offset] = L'\0';
		return *this;
	}

	/*!
	 \brief Read data from the buffer into an object.

	 \param data Where to store the data.

	 \return This object.

	 \overload
	 */
	InputStream &operator>>( std::wstring &data );
};

} // namespace MultiLibrary
