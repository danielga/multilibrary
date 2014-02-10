#pragma once

#include <MultiLibrary/Filesystem/Export.hpp>
#include <MultiLibrary/Common/IOStream.hpp>
#include <string>

namespace MultiLibrary
{

class FileInternal;

class MULTILIBRARY_FILESYSTEM_API File : public IOStream
{
public:
	File( FileInternal *file );
	File( const File &f );
	virtual ~File( );

	typedef void ( *unspecified_bool_type ) ( );
	static void unspecified_bool_true( );

	bool IsValid( ) const;
	operator unspecified_bool_type( ) const;
	bool operator!( ) const;

	bool Close( );

	const std::string &GetPath( ) const;

	int64_t Tell( ) const;
	int64_t Size( ) const;
	bool Seek( int64_t pos, SeekMode mode = SEEKMODE_SET );

	bool Flush( );

	bool Errored( ) const;
	bool EndOfFile( ) const;

	size_t Read( void *value, size_t size );
	size_t Write( const void *value, size_t size );

	File &operator>>( bool &data );
	File &operator>>( int8_t &data );
	File &operator>>( uint8_t &data );
	File &operator>>( int16_t &data );
	File &operator>>( uint16_t &data );
	File &operator>>( int32_t &data );
	File &operator>>( uint32_t &data );
	File &operator>>( int64_t &data );
	File &operator>>( uint64_t &data );
	File &operator>>( float &data );
	File &operator>>( double &data );
	File &operator>>( char &data );
	File &operator>>( char *data ); // This is very dangerous and slow
	File &operator>>( std::string &data ); // This is slow and HDD heavy
	File &operator>>( wchar_t &data );
	File &operator>>( wchar_t *data ); // This is very dangerous and slow
	File &operator>>( std::wstring &data ); // This is slow and HDD heavy

	File &operator<<( const bool &data );
	File &operator<<( const int8_t &data );
	File &operator<<( const uint8_t &data );
	File &operator<<( const int16_t &data );
	File &operator<<( const uint16_t &data );
	File &operator<<( const int32_t &data );
	File &operator<<( const uint32_t &data );
	File &operator<<( const int64_t &data );
	File &operator<<( const uint64_t &data );
	File &operator<<( const float &data );
	File &operator<<( const double &data );
	File &operator<<( const char &data );
	File &operator<<( const char *data );
	File &operator<<( const std::string &data );
	File &operator<<( const wchar_t &data );
	File &operator<<( const wchar_t *data );
	File &operator<<( const std::wstring &data );

	const FileInternal *GetInternal( ) const;

protected:
	FileInternal *file_pointer;
};

} // namespace MultiLibrary