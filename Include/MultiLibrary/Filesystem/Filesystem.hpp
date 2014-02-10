#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <string>
#include <vector>

namespace MultiLibrary
{

class MULTILIBRARY_FILESYSTEM_API Filesystem
{
	friend class FileInternal;

public:
	virtual ~Filesystem( );

	virtual File Open( const std::string &path, const char *mode );
	virtual int64_t Size( const std::string &path );
	virtual bool Exists( const std::string &path );
	virtual bool CreateFile( const std::string &path, bool overwrite = false );
	virtual bool RemoveFile( const std::string &path );

	virtual uint64_t Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders );

	virtual bool IsFolder( const std::string &path );
	virtual bool CreateFolder( const std::string &path );
	virtual bool RemoveFolder( const std::string &path, bool recursive );

	static std::string GetExecutablePath( );

protected:
	virtual bool CloseInternal( const FileInternal *file );

	std::vector<FileInternal *> open_files;
};

} // namespace MultiLibrary