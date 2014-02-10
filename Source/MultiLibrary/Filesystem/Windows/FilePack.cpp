#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Filesystem/FilePack.hpp>
#include <MultiLibrary/Filesystem/FilePacked.hpp>
#include <cstdio>
#include <stdexcept>
#include <sys/stat.h>
#include <windows.h>
#undef CreateFile

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

static void ParseFlags( const char *m, int *page, int *file )
{
	switch( *m++ )
	{
	case 'r':
		*page |= PAGE_READONLY;
		*file |= FILE_MAP_READ;
		break;

	case 'w':
	case 'a':
		*page |= FILE_MAP_WRITE;
		*file |= PAGE_READWRITE;
		break;

	default:
		return;
	}

	if( *m == '+' || ( *m == 'b' && m[1] == '+' ) )
	{
		*page = FILE_MAP_READ | FILE_MAP_WRITE;
		*file = PAGE_READWRITE;
	}
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

			SYSTEM_INFO info;
			GetSystemInfo( &info );
			size_t pagesize = static_cast<size_t>( info.dwAllocationGranularity );

			size_t diff = static_cast<size_t>( offset % pagesize );
			size += diff;
			offset -= diff;

			DWORD offsetLow =

#if defined _WIN64

				static_cast<DWORD>( offset & 0xFFFFFFFFL );

#else

				static_cast<DWORD>( offset );

#endif

			DWORD offsetHigh =

#if defined _WIN64

				static_cast<DWORD>( ( offset >> 32 ) & 0xFFFFFFFFL );

#else

				0;

#endif

			int page_flags = 0;
			int file_flags = 0;
			ParseFlags( mode, &page_flags, &file_flags );

			HANDLE handle = static_cast<FILE *>( file_pointer->GetInternal( ) );
			handle = CreateFileMapping( handle, NULL, page_flags, 0, 0, NULL );
			if( handle == NULL )
				return File( NULL );

			void *map = MapViewOfFile( handle, file_flags, offsetHigh, offsetLow, size );
			CloseHandle( handle );

			if( map == NULL )
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
			UnmapViewOfFile( file->map_pointer );
			delete file;
			open_files.erase( it );
			return true;
		}

	return false;
}

} // namespace MultiLibrary
