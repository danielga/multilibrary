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
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <MultiLibrary/Common/IOStream.hpp>
#include <memory>

namespace MultiLibrary
{

namespace Standard
{

enum class Input
{
	None = -1,
	Normal
};

enum class Output
{
	None = -1,
	Normal,
	Error
};

}

/*!
 \brief A class that wraps the operating system's pipe functionality.
 */
class MULTILIBRARY_COMMON_API Pipe : public IOStream, public NonCopyable
{
public:
	Pipe( Standard::Input in, Standard::Output out );

	/*!
	 \brief Create a pipe with the specified inheritances.

	 \param read_inheritable Should the read handle be inheritable
	 \param write_inheritable Should the write handle be inheritable
	 */
	Pipe( bool read_inheritable, bool write_inheritable );

	/*!
	 \brief Destructor.

	 Subscribers are alerted to this object destruction, if any.
	 */
	~Pipe( );

	bool IsValid( ) const;

	/*!
	 \brief Set the current position for read/write operations.

	 \param position Position value.
	 \param mode (Optional) Type of seeking pretended.

	 \return true if it succeeds, false if it fails.
	 */
	bool Seek( int64_t position, SeekMode mode = SEEKMODE_SET );

	/*!
	 \brief Get the current position.

	 \return Current position.
	 */
	int64_t Tell( ) const;

	/*!
	 \brief Get the size.

	 \return Size of internal data.
	 */
	int64_t Size( ) const;

	/*!
	 \brief Return whether we reached end of file or not.

	 \return true if we reached end of file, false otherwise.
	 */
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

	void CloseRead( );
	void CloseWrite( );

	class Handle;
	const Handle &ReadHandle( ) const;
	const Handle &WriteHandle( ) const;

private:
	std::unique_ptr<Handle> read_handle;
	std::unique_ptr<Handle> write_handle;
};

} // namespace MultiLibrary
