#pragma once

#include <MultiLibrary/Network/Export.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_NETWORK_API SocketError
{
public:
	SocketError( );
	SocketError( int error );

	void Set( int error );
	int Get( ) const;

	void Clear( );

	typedef void ( *unspecified_bool_type ) ( );
	static void unspecified_bool_true( );

	operator unspecified_bool_type( ) const;
	bool operator!( ) const;

	bool operator==( const SocketError &rhs );
	bool operator!=( const SocketError &rhs );

private:
	int error_value;
};

} // namespace MultiLibrary