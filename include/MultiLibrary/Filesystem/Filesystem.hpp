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
#include <MultiLibrary/Filesystem/File.hpp>
#include <string>
#include <vector>

namespace MultiLibrary
{

class MULTILIBRARY_FILESYSTEM_API Filesystem
{
public:
	virtual ~Filesystem( );

	virtual File Open( const std::string &path, const char *mode );
	virtual int64_t Size( const std::string &path );
	virtual bool Exists( const std::string &path );
	virtual bool RemoveFile( const std::string &path );

	virtual uint64_t Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders );

	virtual bool IsFolder( const std::string &path );
	virtual bool CreateFolder( const std::string &path );
	virtual bool RemoveFolder( const std::string &path, bool recursive );

	static std::string GetExecutablePath( );

	virtual bool Close( FileInternal *file );

protected:
	std::vector<FileInternal *> open_files;
};

} // namespace MultiLibrary
