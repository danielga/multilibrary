#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Common/IOStream.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <string>

namespace MultiLibrary
{

class Filesystem;

class MULTILIBRARY_FILESYSTEM_API FileInternal : public NonCopyable
{
protected:
	FileInternal( Filesystem *fsystem, void *file, const std::string &path );
	virtual ~FileInternal( );

public:
	uint32_t AddReference( );
	uint32_t ReleaseReference( );
	void *GetInternal( );

	virtual bool IsValid( ) const = 0;
	virtual const std::string &GetPath( ) const = 0;
	virtual int64_t Tell( ) const = 0;
	virtual int64_t Size( ) const = 0;
	virtual bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET ) = 0;
	virtual bool Flush( ) = 0;
	virtual bool Errored( ) const = 0;
	virtual bool EndOfFile( ) const = 0;

	virtual size_t Read( void *data, size_t size ) = 0;
	virtual size_t Write( const void *data, size_t size ) = 0;

protected:
	Filesystem *parent_filesystem;
	const std::string file_path;
	void *file_pointer;
	uint32_t file_refs;
};

} // namespace MultiLibrary