/*************************************************************************
 * MultiLibrary - http://danielga.github.io/multilibrary/
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

namespace MultiLibrary
{

class FileInternal
{
public:
	virtual ~FileInternal( ) { }

	virtual void *Release( ) = 0;

	virtual bool IsValid( ) const = 0;
	virtual const std::string &GetPath( ) const = 0;
	virtual int64_t Tell( ) const = 0;
	virtual int64_t Size( ) const = 0;
	virtual bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET ) = 0;
	virtual bool Flush( ) = 0;
	virtual bool Errored( ) const = 0;
	virtual bool EndOfFile( ) const = 0;

	virtual size_t Read( void *data, size_t size ) = 0;
	virtual int Scan( const char *format, ... ) = 0;
	virtual size_t Write( const void *data, size_t size ) = 0;
	virtual int Print( const char *format, ... ) = 0;
};

} // namespace MultiLibrary
