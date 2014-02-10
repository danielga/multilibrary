#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <mach-o/dyld.h>

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
	FILE *file = fopen( path.c_str( ), mode );
	if( file == NULL )
		return File( NULL );

	FileSimple *finternal = new FileSimple( this, file, path );
	open_files.push_back( finternal );
	return File( finternal );
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
	return unlink( path.c_str( ) ) == 0;
}

static std::string __filter_file;
static int __scandir_filter( const struct dirent *dir )
{
	return strcasecmp( dir->d_name, __filter_file.c_str( ) ) == 0 ? 1 : 0;
}

uint64_t Filesystem::Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders )
{
	std::string find_copy;
	find_copy.assign( find );
	unsigned int pos = 0;
	while( ( pos = find_copy.find( '\\' ) ) != std::string::npos )
		find_copy.replace( pos, pos, "/" );

	while( ( pos = find_copy.find( "//" ) ) != std::string::npos )
		find_copy.replace( pos, pos, "/" );

	pos = find_copy.rfind( '/' );
	if( pos != std::string::npos )
		__filter_file.assign( find_copy, 0, pos );
	else
		__filter_file = find_copy;

	struct dirent **namelist;
	int num = scandir( find_copy.c_str( ), &namelist, __scandir_filter, alphasort );
	if( num == -1 )
		return 0;



	__filter_file = std::string( );

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

	rmdir( path.c_str( ) );
	return true;
}

std::string Filesystem::GetExecutablePath( )
{
	std::string execPath;
	execPath.resize( 1 );
	uint32_t bytes = 1;
	if( _NSGetExecutablePath( &execPath[0], &bytes ) == -1 )
	{
		execPath.resize( bytes + 1 );
		_NSGetExecutablePath( &execPath[0], &bytes );
		execPath.resize( bytes );
	}

	return execPath;
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
