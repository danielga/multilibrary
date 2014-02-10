#include <MultiLibrary/Network/SocketError.hpp>

namespace MultiLibrary
{

SocketError::SocketError( ) :
	error_value( 0 )
{ }

SocketError::SocketError( int error ) :
	error_value( error )
{ }

void SocketError::Set( int error )
{
	error_value = error;
}

int SocketError::Get( ) const
{
	return error_value;
}

void SocketError::Clear( )
{
	error_value = 0;
}

void SocketError::unspecified_bool_true( ) { }

SocketError::operator unspecified_bool_type( ) const
{
	return error_value == 0 ? 0 : unspecified_bool_true;
}

bool SocketError::operator!( ) const
{
	return error_value == 0;
}

bool SocketError::operator==( const SocketError &rhs )
{
	return error_value == rhs.error_value;
}

bool SocketError::operator!=( const SocketError &rhs )
{
	return error_value != rhs.error_value;
}

} // namespace MultiLibrary
