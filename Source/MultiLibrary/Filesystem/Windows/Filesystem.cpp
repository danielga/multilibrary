#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <windows.h>
#include <direct.h>
#undef CreateFile

namespace MultiLibrary
{

Filesystem::~Filesystem( )
{
	for( size_t k = 0; k < open_files.size( ); ++k )
		CloseInternal( open_files[k] );

	open_files.clear( );
}

File Filesystem::Open( const std::string &path, const char *mode )
{
	FILE *file = NULL;
	if( fopen_s( &file, path.c_str( ), mode ) != 0 || file == NULL )
		return File( NULL );

	FileSimple *finternal = new FileSimple( this, file, path );
	open_files.push_back( finternal );
	return File( finternal );
}

int64_t Filesystem::Size( const std::string &path )
{
	struct _stat64 stat;
	if( _stat64( path.c_str( ), &stat ) == 0 )
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

bool Filesystem::CreateFile( const std::string &path, bool overwrite )
{
	if( overwrite || !Exists( path ) )
	{
		File file = Open( path, "w" );
		file.Close( );
		return true;
	}

	return false;
}

bool Filesystem::RemoveFile( const std::string &path )
{
	return _unlink( path.c_str( ) ) == 0;
}

uint64_t Filesystem::Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders )
{
	WIN32_FIND_DATAA find_data;
	uint64_t num_files = 0;

	HANDLE find_handle = FindFirstFileEx( find.c_str( ), FindExInfoStandard, &find_data, FindExSearchNameMatch, NULL, 0 );
	while( find_handle != INVALID_HANDLE_VALUE )
	{
		std::string filename = find_data.cFileName;
		if( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( filename != "." && filename != ".." )
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

		if( !FindNextFile( find_handle, &find_data ) )
			break;
	}

	FindClose( find_handle );
	return num_files;
}

bool Filesystem::IsFolder( const std::string &path )
{
	struct _stat64 stat;
	if( _stat64( path.c_str( ), &stat ) != 0 )
		return false;

	return ( stat.st_mode & S_IFMT ) == S_IFDIR;
}

bool Filesystem::CreateFolder( const std::string &path )
{
	return _mkdir( path.c_str( ) ) == 0;
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

	_rmdir( path.c_str( ) );
	return true;
}

std::string Filesystem::GetExecutablePath( )
{
	char *path = NULL;
	if( _get_pgmptr( &path ) != 0 || path == NULL )
		return std::string( );

	return path;
}

bool Filesystem::CloseInternal( const FileInternal *f )
{
	if( f == NULL )
		return false;

	std::vector<FileInternal *>::iterator it, end = open_files.end( );
	for( it = open_files.begin( ); it != end; ++it )
		if( *it == f )
		{
			FileSimple *file = static_cast<FileSimple *>( *it );
			fclose( static_cast<FILE *>( file->file_pointer ) );
			delete file;
			open_files.erase( it );
			return true;
		}

	return false;
}

} // namespace MultiLibrary
