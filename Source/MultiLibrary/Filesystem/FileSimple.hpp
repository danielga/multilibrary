#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Filesystem/FileInternal.hpp>
#include <string>

namespace MultiLibrary
{

class FileSystem;
class FilePack;

class MULTILIBRARY_FILESYSTEM_API FileSimple : public FileInternal
{
	friend Filesystem;

protected:
	FileSimple( Filesystem *fsystem, void *file, const std::string &path );
	~FileSimple( ) { }

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
};

} // namespace MultiLibrary