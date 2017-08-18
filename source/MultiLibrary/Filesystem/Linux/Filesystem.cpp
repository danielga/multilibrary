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

#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <MultiLibrary/Common/String.hpp>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <strings.h>

namespace MultiLibrary
{

Filesystem::~Filesystem( )
{
	for( size_t k = 0; k < open_files.size( ); ++k )
		Close( open_files[k] );

	open_files.clear( );
}

File Filesystem::Open( const std::string &path, const char *mode )
{
	FILE *file = fopen( path.c_str( ), mode );
	if( file == nullptr )
		return File( std::shared_ptr<FileInternal>( ) );

	FileSimple *finternal = new FileSimple( this, file, path );
	open_files.push_back( finternal );
	return File( std::shared_ptr<FileInternal>( finternal ) );
}

int64_t Filesystem::Size( const std::string &path )
{
	struct stat64 stat;
	if( stat64( path.c_str( ), &stat ) == 0 )
		return stat.st_size;

	return -1;
}

bool Filesystem::Exists( const std::string &path )
{
	File file = Open( path, "r" );
	if( file.IsValid( ) )
	{
		file.Close( );
		return true;
	}

	return false;
}

bool Filesystem::RemoveFile( const std::string &path )
{
	return unlink( path.c_str( ) ) == 0;
}

uint64_t Filesystem::Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders )
{
	struct dirent **namelist;
	int num = scandir( find.c_str( ), &namelist, nullptr, alphasort );
	if( num == -1 )
		return 0;

	std::string short_find = find;
	if( short_find.back( ) == '/' )
		short_find.erase( short_find.size( ) - 1 );

	size_t pos = find.rfind( '/' );
	if( pos != find.npos )
		short_find = find.substr( pos + 1 );

	for( std::vector<std::string>::iterator it = files.begin( ); it != files.end( ); ++it )
	{
		if( !String::WildcardCompare( *it, find ) )
			it = files.erase( it );
	}

	for( std::vector<std::string>::iterator it = folders.begin( ); it != folders.end( ); ++it )
	{
		if( !String::WildcardCompare( *it, find ) )
			it = folders.erase( it );
	}

	return 0;
}

bool Filesystem::IsFolder( const std::string &path )
{
	struct stat64 stat;
	if( stat64( path.c_str( ), &stat ) != 0 )
		return false;

	return ( stat.st_mode & S_IFMT ) == S_IFDIR;
}

bool Filesystem::CreateFolder( const std::string &path )
{
	return mkdir( path.c_str( ), S_IRWXU | S_IRWXG | S_IRWXO ) == 0;
}

bool Filesystem::RemoveFolder( const std::string &path, bool recursive )
{
	if( !IsFolder( path ) )
		return false;

	if( recursive )
	{
		std::vector<std::string> files;
		std::vector<std::string> folders;
		Find( path + "/*", files, folders );

		for( size_t k = 0; k != files.size( ); ++k )
			if ( !RemoveFile( path + "/" + files[k] ) )
				return false;

		for( size_t k = 0; k < folders.size( ); ++k )
			if ( !RemoveFolder( path + "/" + folders[k], true ) )
				return false;
	}

	return rmdir( path.c_str( ) ) == 0;
}

std::string Filesystem::GetExecutablePath( )
{
	std::string execPath;
	struct stat sb;
	if( lstat( "/proc/self/exe", &sb ) == -1 )
		return std::string( );

	execPath.resize( sb.st_size + 1 );
	ssize_t bytes = readlink( "/proc/self/exe", &execPath[0], sb.st_size + 1 );
	if( bytes < 0 || bytes > sb.st_size )
		return std::string( );

	execPath[bytes] = '\0';
	execPath.resize( bytes );
	return execPath;
}

bool Filesystem::Close( FileInternal *file )
{
	if( file == nullptr )
		return false;

	std::vector<FileInternal *>::iterator it, end = open_files.end( );
	for( it = open_files.begin( ); it != end; ++it )
		if( *it == file )
		{
			fclose( static_cast<FILE *>( file->Release( ) ) );
			open_files.erase( it );
			return true;
		}

	return false;
}

} // namespace MultiLibrary
