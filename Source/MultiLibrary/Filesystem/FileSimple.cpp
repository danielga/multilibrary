#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/FileSimple.hpp>
#include <cstdio>
#include <cassert>
#include <sys/stat.h>

#if defined _WIN32

	#define ftello64 _ftelli64
	#define stat64 _stat64
	#define fstat64 _fstat64
	#define fseeko64 _fseeki64
	#define fileno _fileno

#endif

namespace MultiLibrary
{

FileSimple::FileSimple( Filesystem *fsystem, void *file, const std::string &path ) :
	FileInternal( fsystem, file, path )
{ }

bool FileSimple::IsValid( ) const
{
	return !Errored( ) && !EndOfFile( );
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
	assert( data != NULL && size != 0 );

	return fread( data, 1, size, static_cast<FILE *>( file_pointer ) );
}

size_t FileSimple::Write( const void *data, size_t size )
{
	assert( data != NULL && size != 0 );

	return fwrite( data, 1, size, static_cast<FILE *>( file_pointer ) );
}

} // namespace MultiLibrary