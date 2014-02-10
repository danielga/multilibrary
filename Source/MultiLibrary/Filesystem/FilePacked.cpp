#include <MultiLibrary/Filesystem/FilePacked.hpp>
#include <MultiLibrary/Filesystem/FilePack.hpp>
#include <cstring>

#if defined _WIN32

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

#elif defined __APPLE__ || defined __linux

	#include <sys/mman.h>

#endif

namespace MultiLibrary
{

FilePacked::FilePacked( FilePack *fpack, void *file, int64_t size, int64_t offset, const std::string &path ) :
	FileInternal( fpack, static_cast<char *>( file ) + offset, path ),
	errored( false ),
	end_of_file( false ),
	file_size( size ),
	file_offset( 0 ),
	map_pointer( file ),
	map_size( size + offset )
{ }

bool FilePacked::IsValid( ) const
{
	return !Errored( ) && !EndOfFile( );
}

const std::string &FilePacked::GetPath( ) const
{
	return file_path;
}

int64_t FilePacked::Tell( ) const
{
	return file_offset;
}

int64_t FilePacked::Size( ) const
{
	return file_size;
}

bool FilePacked::Seek( int64_t pos, SeekMode mode )
{
	switch( mode )
	{
	case SEEKMODE_SET:
		file_offset = pos;
		break;

	case SEEKMODE_CUR:
		file_offset += pos;
		break;

	case SEEKMODE_END:
		file_offset = file_size + pos;
		break;

	default:
		return false;
	}

	errored = false;
	end_of_file = false;

	return true;
}

bool FilePacked::Flush( )
{

#if defined _WIN32
	
	return FlushViewOfFile( file_pointer, static_cast<size_t>( file_size ) ) == TRUE;

#else

	return msync( file_pointer, static_cast<size_t>( file_size ), MS_ASYNC ) == 0;

#endif

}

bool FilePacked::Errored( ) const
{
	return errored;
}

bool FilePacked::EndOfFile( ) const
{
	return end_of_file;
}

size_t FilePacked::Read( void *data, size_t size )
{
	if( file_offset >= file_size )
	{
		end_of_file = true;
		return 0;
	}

	const char *filedata = static_cast<const char *>( file_pointer );
	size_t read = static_cast<size_t>( file_size - file_offset );
	if( read > size )
		read = size;

	std::memcpy( data, &filedata[file_offset], read );
	file_offset += read;
	if( read < size )
		end_of_file = true;

	return read;
}

size_t FilePacked::Write( const void *data, size_t size )
{
	if( file_offset >= file_size )
	{
		errored = true;
		return 0;
	}

	char *filedata = static_cast<char *>( file_pointer );
	size_t write = static_cast<size_t>( file_size - file_offset );
	if( write > size )
		write = size;

	std::memcpy( &filedata[file_offset], data, write );
	file_offset += write;
	if( write < size )
		errored = true;

	return write;
}

} // namespace MultiLibrary
