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
#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <windows.h>
#include <direct.h>

#undef CreateFile

namespace MultiLibrary
{

namespace Internal
{

static uint64_t Find( const std::wstring &widefind, std::vector<std::wstring> &files, std::vector<std::wstring> &folders )
{
	WIN32_FIND_DATA find_data;
	uint64_t num_files = 0;

	HANDLE find_handle = FindFirstFileEx( widefind.c_str( ), FindExInfoStandard, &find_data, FindExSearchNameMatch, nullptr, 0 );
	while( find_handle != INVALID_HANDLE_VALUE )
	{
		std::wstring filename = find_data.cFileName;
		if( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( filename != L"." && filename != L".." )
			{
				folders.push_back( filename );
				++num_files;
			}
		}
		else
		{
			files.push_back( filename );
			++num_files;
		}

		if( FindNextFile( find_handle, &find_data ) == FALSE )
			break;
	}

	FindClose( find_handle );
	return num_files;
}

static bool IsFolder( const std::wstring &widepath )
{
	struct _stat64 stat;
	if( _wstat64( widepath.c_str( ), &stat ) != 0 )
		return false;

	return ( stat.st_mode & S_IFMT ) == S_IFDIR;
}

static bool RemoveFolder( const std::wstring &widepath, bool recursive )
{
	if( !IsFolder( widepath ) )
		return false;

	if( recursive )
	{
		std::vector<std::wstring> files;
		std::vector<std::wstring> folders;
		Find( widepath + L"/*", files, folders );

		for( size_t k = 0; k != files.size( ); ++k )
			if ( _wunlink( widepath.c_str( ) ) != 0 )
				return false;

		for( size_t k = 0; k < folders.size( ); ++k )
			if ( !RemoveFolder( widepath + L"/" + folders[k], true ) )
				return false;
	}

	return _wrmdir( widepath.c_str( ) ) == 0;
}

}

Filesystem::~Filesystem( )
{
	std::vector<FileInternal *>::iterator it, end = open_files.end( );
	for( it = open_files.begin( ); it != end; ++it )
		fclose( static_cast<FILE *>( ( *it )->Release( ) ) );
}

File Filesystem::Open( const std::string &path, const char *mode )
{
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );

	std::wstring widemode;
	UTF16::FromUTF8( mode, mode + strlen( mode ), std::back_inserter( widemode ) );

	FILE *file = _wfopen( widepath.c_str( ), widemode.c_str( ) );
	if( file == nullptr )
		return File( std::shared_ptr<FileInternal>( ) );

	FileSimple *finternal = new FileSimple( this, file, path );
	open_files.push_back( finternal );
	return File( std::shared_ptr<FileInternal>( finternal ) );
}

int64_t Filesystem::Size( const std::string &path )
{
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );

	struct _stat64 stat;
	if( _wstat64( widepath.c_str( ), &stat ) == 0 )
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
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );
	return _wunlink( widepath.c_str( ) ) == 0;
}

uint64_t Filesystem::Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders )
{
	std::wstring widefind;
	UTF16::FromUTF8( find.begin( ), find.end( ), std::back_inserter( widefind ) );

	std::vector<std::wstring> wfiles;
	std::vector<std::wstring> wfolders;
	uint64_t num_files = Internal::Find( widefind, wfiles, wfolders );

	files.reserve( wfiles.size( ) );
	folders.reserve( wfolders.size( ) );

	for( std::vector<std::wstring>::iterator it = wfiles.begin( ); it != wfiles.end( ); ++it )
	{
		std::wstring &wfile = *it;
		std::string file;
		UTF8::FromUTF16( wfile.begin( ), wfile.end( ), std::back_inserter( file ) );
		files.push_back( file );
	}

	for( std::vector<std::wstring>::iterator it = wfolders.begin( ); it != wfolders.end( ); ++it )
	{
		std::wstring &wfolder = *it;
		std::string folder;
		UTF8::FromUTF16( wfolder.begin( ), wfolder.end( ), std::back_inserter( folder ) );
		folders.push_back( folder );
	}

	return num_files;
}

bool Filesystem::IsFolder( const std::string &path )
{
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );
	return Internal::IsFolder( widepath );
}

bool Filesystem::CreateFolder( const std::string &path )
{
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );
	return _wmkdir( widepath.c_str( ) ) == 0;
}

bool Filesystem::RemoveFolder( const std::string &path, bool recursive )
{
	std::wstring widepath;
	UTF16::FromUTF8( path.begin( ), path.end( ), std::back_inserter( widepath ) );
	return Internal::RemoveFolder( widepath, recursive );
}

std::string Filesystem::GetExecutablePath( )
{
	std::string path;
	wchar_t *widepath = _wpgmptr;
	if( widepath == nullptr )
		return path;

	UTF8::FromUTF16( widepath, widepath + wcslen( widepath ), std::back_inserter( path ) );
	return path;
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
