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

#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <cassert>
#include <cstdarg>
#include <sys/stat.h>

#if defined _WIN32 && !defined __GNUC__

	#define ftello64 _ftelli64
	#define stat64 _stat64
	#define fstat64 _fstat64
	#define fseeko64 _fseeki64
	#define fileno _fileno

#endif

namespace MultiLibrary
{

FileSimple::FileSimple( Filesystem *fsystem, FILE *file, const std::string &path ) :
	parent_filesystem( fsystem ),
	file_pointer( file ),
	file_path( path )
{ }

FileSimple::~FileSimple( )
{
	if( file_pointer != nullptr && parent_filesystem != nullptr )
		parent_filesystem->Close( this );
}

void *FileSimple::Release( )
{
	parent_filesystem = nullptr;
	void *file = file_pointer;
	file_pointer = nullptr;
	return file;
}

bool FileSimple::IsValid( ) const
{
	return file_pointer != nullptr && !Errored( ) && !EndOfFile( );
}

const std::string &FileSimple::GetPath( ) const
{
	return file_path;
}

int64_t FileSimple::Tell( ) const
{
	return ftello64( static_cast<FILE *>( file_pointer ) );
}

int64_t FileSimple::Size( ) const
{
	struct stat64 stats;
	fstat64( fileno( static_cast<FILE *>( file_pointer ) ), &stats );
	return stats.st_size;
}

bool FileSimple::Seek( int64_t pos, SeekMode mode )
{
	return fseeko64( static_cast<FILE *>( file_pointer ), pos, mode ) == 0;
}

bool FileSimple::Flush( )
{
	return fflush( static_cast<FILE *>( file_pointer ) ) == 0;
}

bool FileSimple::Errored( ) const
{
	return ferror( static_cast<FILE *>( file_pointer ) ) != 0;
}

bool FileSimple::EndOfFile( ) const
{
	return feof( static_cast<FILE *>( file_pointer ) ) != 0;
}

size_t FileSimple::Read( void *data, size_t size )
{
	assert( data != nullptr && size != 0 );

	return fread( data, 1, size, static_cast<FILE *>( file_pointer ) );
}

int FileSimple::Scan( const char *format, ... )
{
	assert( format != nullptr );

	va_list args;
	va_start( args, format );
	int num = vfscanf( static_cast<FILE *>( file_pointer ), format, args );
	va_end( args );
	return num;
}

size_t FileSimple::Write( const void *data, size_t size )
{
	assert( data != nullptr && size != 0 );

	return fwrite( data, 1, size, static_cast<FILE *>( file_pointer ) );
}

int FileSimple::Print( const char *format, ... )
{
	assert( format != nullptr );

	va_list args;
	va_start( args, format );
	int num = vfprintf( static_cast<FILE *>( file_pointer ), format, args );
	va_end( args );
	return num;
}

} // namespace MultiLibrary