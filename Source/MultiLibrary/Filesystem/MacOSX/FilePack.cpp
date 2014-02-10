#include <MultiLibrary/Filesystem/FilePack.hpp>
#include <MultiLibrary/Filesystem/FilePacked.hpp>
#include <cstdio>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/mman.h>

#if defined _WIN32

	#define stat64 _stat64
	#define fstat64 _fstat64
	#define fileno _fileno

#elif defined __APPLE__ || defined __linux

	#include <unistd.h>

#endif

namespace MultiLibrary
{

FilePack::FilePack( const FilePack &f ) :
	File( f ),
	files_list( f.files_list )
{
	open_files = f.open_files;
}

FilePack::FilePack( const File &f ) :
	File( f ),
	files_list( )
{
	Seek( 0 );
	std::string header( 11, '\0' );
	header.resize( Read( &header[0], 11 ) );
	if( header != "FILEPACK001" )
		throw std::runtime_error( "file opened is not a filepack" );

	uint8_t pathlen = 0;
	Entry entry;
	entry.path.resize( 256 );
	Read( &entry.offset, sizeof( entry.offset ) );
	while( entry.offset != 0 )
	{
		Read( &entry.size, sizeof( entry.size ) );
		Read( &pathlen, 1 );
		entry.path.resize( Read( &entry.path[0], pathlen ) );
		files_list.push_back( entry );
		Read( &entry.offset, sizeof( entry.offset ) );
	}
}

FilePack::~FilePack( )
{
	for( size_t k = 0; k < open_files.size( ); ++k )
		CloseInternal( open_files[k] );

	open_files.clear( );
	files_list.clear( );
}

static int ParseFlags( const char *m )
{
	int prot = PROT_NONE;

	switch( *m++ )
	{
	case 'r':
		prot |= PROT_READ;
		break;

	case 'w':
	case 'a':
		prot |= PROT_WRITE;
		break;

	default:
		return prot;
	}

	if( *m == '+' || ( *m == 'b' && m[1] == '+' ) )
		prot = PROT_READ | PROT_WRITE;

	return prot;
}

File FilePack::Open( const std::string &path, const char *mode )
{
	std::vector<Entry>::const_iterator it, end = files_list.end( );
	for( it = files_list.begin( ); it != end; ++it )
	{
		const Entry &entry = *it;
		if( entry.path == path )
		{
			size_t size = static_cast<size_t>( entry.size );
			size_t offset = static_cast<size_t>( entry.offset );

			size_t diff = static_cast<size_t>( offset % getpagesize( ) );
			size += diff;
			offset -= diff;

			int file_num = fileno( static_cast<FILE *>( file_pointer->GetInternal( ) ) );

			void *map = mmap( NULL, size, ParseFlags( mode ), 0, file_num, offset );
			if( map == MAP_FAILED )
				return File( NULL );

			FilePacked *finternal = new FilePacked( this, map, entry.size, diff, path );
			open_files.push_back( finternal );
			return File( finternal );
		}
	}

	return File( NULL );
}

int64_t FilePack::Size( const std::string &path )
{
	std::vector<Entry>::const_iterator it, end = files_list.end( );
	for( it = files_list.begin( ); it != end; ++it )
	{
		const Entry &entry = *it;
		if( entry.path == path )
			return static_cast<int64_t>( entry.size );
	}

	return 0;
}

bool FilePack::Exists( const std::string &path )
{
	std::vector<Entry>::const_iterator it, end = files_list.end( );
	for( it = files_list.begin( ); it != end; ++it )
		if( ( *it ).path == path )
			return true;

	return false;
}

bool FilePack::CreateFile( const std::string &, bool )
{
	return false;
}

bool FilePack::RemoveFile( const std::string & )
{
	return false;
}

uint64_t FilePack::Find( const std::string &, std::vector<std::string> &, std::vector<std::string> & )
{
	return 0;
}

bool FilePack::IsFolder( const std::string & )
{
	return false;
}

bool FilePack::CreateFolder( const std::string & )
{
	return false;
}

bool FilePack::RemoveFolder( const std::string &, bool )
{
	return false;
}

bool FilePack::CloseInternal( const FileInternal *f )
{
	if( f == NULL )
		return false;

	std::vector<FileInternal *>::iterator it, end = open_files.end( );
	for( it = open_files.begin( ); it != end; ++it )
		if( *it == f )
		{
			FilePacked *file = static_cast<FilePacked *>( *it );
			munmap( file->map_pointer, static_cast<size_t>( file->map_size ) );
			delete file;
			open_files.erase( it );
			return true;
		}

	return false;
}

} // namespace MultiLibrary
