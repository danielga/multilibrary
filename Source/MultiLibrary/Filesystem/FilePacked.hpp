#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Filesystem/FileInternal.hpp>
#include <string>

namespace MultiLibrary
{

class FilePack;

class MULTILIBRARY_FILESYSTEM_API FilePacked : public FileInternal
{
	friend FilePack;

protected:
	FilePacked( FilePack *fpack, void *file, int64_t size, int64_t offset, const std::string &path );
	~FilePacked( ) { }

public:
	bool IsValid( ) const;
	const std::string &GetPath( ) const;
	int64_t Tell( ) const;
	int64_t Size( ) const;
	bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET );
	bool Flush( );
	bool Errored( ) const;
	bool EndOfFile( ) const;

	size_t Read( void *data, size_t size );
	size_t Write( const void *data, size_t size );

protected:
	bool errored;
	bool end_of_file;
	int64_t file_size;
	int64_t file_offset;
	void *map_pointer;
	int64_t map_size;
};

} // namespace MultiLibrary