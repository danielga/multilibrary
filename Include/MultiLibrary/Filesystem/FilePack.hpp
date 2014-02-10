#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <vector>

namespace MultiLibrary
{

class FilePacked;

class MULTILIBRARY_FILESYSTEM_API FilePack : public File, public Filesystem
{
public:
	FilePack( const FilePack &f );
	FilePack( const File &f );
	~FilePack( );

	File Open( const std::string &path, const char *mode );
	int64_t Size( const std::string &path );
	bool Exists( const std::string &path );
	bool CreateFile( const std::string &path, bool overwrite = false );
	bool RemoveFile( const std::string &path );

	uint64_t Find( const std::string &find, std::vector<std::string> &files, std::vector<std::string> &folders );

	bool IsFolder( const std::string &path );
	bool CreateFolder( const std::string &path );
	bool RemoveFolder( const std::string &path, bool recursive );

private:
	bool CloseInternal( const FileInternal *file );

	struct Entry
	{
		std::string path;
		uint64_t size;
		uint64_t offset;
	};

	std::vector<Entry> files_list;
};

} // namespace MultiLibrary