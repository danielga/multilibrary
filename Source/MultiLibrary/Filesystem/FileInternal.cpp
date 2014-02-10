#include <MultiLibrary/Filesystem/FileInternal.hpp>
#include <MultiLibrary/Filesystem/Filesystem.hpp>

namespace MultiLibrary
{

FileInternal::FileInternal( Filesystem *fsystem, void *file, const std::string &path ) :
	parent_filesystem( fsystem ),
	file_path( path ),
	file_pointer( file ),
	file_refs( 0 )
{ }

FileInternal::~FileInternal( ) { }

uint32_t FileInternal::AddReference( )
{
	return ++file_refs;
}

uint32_t FileInternal::ReleaseReference( )
{
	if( --file_refs == 0 )
		parent_filesystem->CloseInternal( this );

	return file_refs;
}

void *FileInternal::GetInternal( )
{
	return file_pointer;
}

} // namespace MultiLibrary