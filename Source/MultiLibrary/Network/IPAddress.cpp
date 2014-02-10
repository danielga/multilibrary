#include <MultiLibrary/Network/IPAddress.hpp>
#include <cstring>

#if defined _WIN32

	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <ws2tcpip.h>
	#define s6_addr16 _S6_un
	typedef u_short sa_family_t;

#elif defined __APPLE__ || defined __linux

	#include <netdb.h>
	#include <netinet/in.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	
#endif

namespace MultiLibrary
{

static const int __little_endian_t__ = 1;
static const bool __little_endian__ = *reinterpret_cast<const char *>( &__little_endian_t__ ) == 1;
template<typename T> void NetworkCopy( T *dst, const T *src, size_t len )
{
	const uint8_t *source = reinterpret_cast<const uint8_t *>( src );
	uint8_t *destination = reinterpret_cast<uint8_t *>( dst );
	size_t bytes = len * sizeof( T );

	if( __little_endian__ )
		for( size_t k = 0; k <= bytes; k += sizeof( T ) )
			NetworkCopy( &destination[k], &source[bytes - k - 1], sizeof( T ) );
	else
		std::memcpy( destination, source, bytes );
}

IPAddress::IPAddress( IPAddressFamily family ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetFamily( family );
}

IPAddress::IPAddress( const std::string &address, uint16_t port ) :
	host_address( NULL )
{
	InitializeAddress( );
	ResolveString( address );
	SetPort( port );
}

IPAddress::IPAddress( const sockaddr_in &address ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetIPAddress( address );
}

IPAddress::IPAddress( uint32_t address, uint16_t port ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetAddress( address );
	SetPort( port );
}

IPAddress::IPAddress( const uint8_t address[4], uint16_t port ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetAddress( address );
	SetPort( port );
}

IPAddress::IPAddress( const sockaddr_in6 &address ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetIPAddress( address );
}

IPAddress::IPAddress( const uint16_t address[8], uint16_t port ) :
	host_address( NULL )
{
	InitializeAddress( );
	SetAddress( address );
	SetPort( port );
}

IPAddress::~IPAddress( )
{
	delete host_address;
}

IPAddress &IPAddress::operator=( const IPAddress &address )
{
	std::memcpy( host_address, address.host_address, sizeof( sockaddr_storage ) );
	return *this;
}

void IPAddress::SetIPAddress( const sockaddr_in &address )
{
	SetFamily( FAMILY_INET );
	std::memcpy( host_address, &address, sizeof( sockaddr_in ) );
}

void IPAddress::SetIPAddress( const sockaddr_in6 &address )
{
	SetFamily( FAMILY_INET6 );
	std::memcpy( host_address, &address, sizeof( sockaddr_in6 ) );
}

void IPAddress::SetAddress( const std::string &address )
{
	ResolveString( address );
}

void IPAddress::SetAddress( uint32_t address )
{
	SetFamily( FAMILY_INET );
	reinterpret_cast<sockaddr_in *>( host_address )->sin_addr.s_addr = htonl( address );
}

void IPAddress::SetAddress( const uint8_t address[4] )
{
	SetFamily( FAMILY_INET );
	uint32_t haddress = ( address[0] << 24 ) | ( address[1] << 16 ) | ( address[2] << 8 ) | address[3];
	reinterpret_cast<sockaddr_in *>( host_address )->sin_addr.s_addr = htonl( haddress );
}

void IPAddress::SetAddress( const uint16_t address[8] )
{
	SetFamily( FAMILY_INET6 );
	sockaddr_in6 *haddress = reinterpret_cast<sockaddr_in6 *>( host_address );
	NetworkCopy( reinterpret_cast<uint16_t *>( &haddress->sin6_addr.s6_addr16 ), address, 8 );
}

void IPAddress::SetPort( uint16_t port )
{
	switch( GetFamily( ) )
	{
	case FAMILY_INET:
		reinterpret_cast<sockaddr_in *>( host_address )->sin_port = htons( port );
		break;

	case FAMILY_INET6:
		reinterpret_cast<sockaddr_in6 *>( host_address )->sin6_port = htons( port );
		break;
	}
}

uint32_t IPAddress::GetAddress( ) const
{
	return ntohl( reinterpret_cast<sockaddr_in *>( host_address )->sin_addr.s_addr );
}

void IPAddress::GetAddress( uint8_t address[4] ) const
{
	sockaddr_in *haddress = reinterpret_cast<sockaddr_in *>( host_address );
	NetworkCopy( address, reinterpret_cast<uint8_t *>( haddress->sin_addr.s_addr ), 4 );
}

void IPAddress::GetAddress( uint16_t address[8] ) const
{
	sockaddr_in6 *haddress = reinterpret_cast<sockaddr_in6 *>( host_address );
	NetworkCopy( address, reinterpret_cast<uint16_t *>( &haddress->sin6_addr.s6_addr16 ), 8 );
}

unsigned short IPAddress::GetPort( ) const
{
	switch( GetFamily( ) )
	{
	case FAMILY_INET:
		return ntohs( reinterpret_cast<sockaddr_in *>( host_address )->sin_port );

	case FAMILY_INET6:
		return ntohs( reinterpret_cast<sockaddr_in6 *>( host_address )->sin6_port );
	}

	return 0;
}

const sockaddr *IPAddress::ToSocketAddress( ) const
{
	return reinterpret_cast<const sockaddr *>( host_address );
}

sockaddr *IPAddress::GetSocketAddress( )
{
	return reinterpret_cast<sockaddr *>( host_address );
}

size_t IPAddress::GetAddressSize( ) const
{
	switch( GetFamily( ) )
	{
	case FAMILY_INET:
		return sizeof( sockaddr_in );

	case FAMILY_INET6:
		return sizeof( sockaddr_in6 );
	}

	return 0;
}

std::string IPAddress::ToString( ) const
{
	std::string address;
	switch( GetFamily( ) )
	{
		case FAMILY_INET:
		{
			address.resize( INET_ADDRSTRLEN );

			in_addr *haddress = &reinterpret_cast<sockaddr_in *>( host_address )->sin_addr;
			if( inet_ntop( AF_INET, haddress, &address[0], INET_ADDRSTRLEN ) != NULL )
			{
				address.resize( strlen( address.c_str( ) ) );
				return address;
			}

			break;
		}

		case FAMILY_INET6:
		{
			address.resize( INET6_ADDRSTRLEN );

			in6_addr *haddress = &reinterpret_cast<sockaddr_in6 *>( host_address )->sin6_addr;
			if( inet_ntop( AF_INET6, haddress, &address[0], INET6_ADDRSTRLEN ) != NULL )
			{
				address.resize( strlen( address.c_str( ) ) );
				return address;
			}

			break;
		}
	}

	return address;
}

void IPAddress::SetFamily( IPAddressFamily family )
{
	sockaddr *haddress = reinterpret_cast<sockaddr *>( host_address );
	haddress->sa_family = static_cast<sa_family_t>( family );
}

IPAddressFamily IPAddress::GetFamily( ) const
{
	return IPAddressFamily( reinterpret_cast<sockaddr *>( host_address )->sa_family );
}

void IPAddress::InitializeAddress( )
{
	host_address = new sockaddr_storage;
	std::memset( host_address, 0, sizeof( sockaddr_storage ) );
}

bool IPAddress::ResolveString( const std::string &address )
{
	in_addr addr;
	if( inet_pton( AF_INET, address.c_str( ), &addr ) == 1 )
	{
		// Doesn't call SetAddress because it does byte order stuff and we don't need it.
		SetFamily( FAMILY_INET );
		reinterpret_cast<sockaddr_in *>( host_address )->sin_addr.s_addr = addr.s_addr;
		return true;
	}

	in6_addr addr6;
	if( inet_pton( AF_INET6, address.c_str( ), &addr6 ) == 1 )
	{
		// Doesn't call SetAddress because it does byte order stuff and we don't need it.
		SetFamily( FAMILY_INET6 );
		uint8_t *haddress = reinterpret_cast<sockaddr_in6 *>( host_address )->sin6_addr.s6_addr;
		std::memcpy( haddress, addr6.s6_addr, 16 );
		return true;
	}

	addrinfo *result = 0;

	addrinfo hints;
	std::memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_INET;
	if( getaddrinfo( address.c_str( ), NULL, &hints, &result ) == 0 && result != NULL )
	{
		// Doesn't call SetAddress because it does byte order stuff and we don't need it.
		SetFamily( FAMILY_INET );
		sockaddr_in *haddress = reinterpret_cast<sockaddr_in *>( host_address );
		sockaddr_in *raddress = reinterpret_cast<sockaddr_in *>( result->ai_addr );
		haddress->sin_addr.s_addr = raddress->sin_addr.s_addr;

		freeaddrinfo( result );
		result = NULL;
		return true;
	}

	addrinfo hints6;
	std::memset( &hints6, 0, sizeof( hints6 ) );
	hints6.ai_family = AF_INET6;
	if( getaddrinfo( address.c_str( ), NULL, &hints6, &result ) == 0 && result != NULL )
	{
		// Doesn't call SetAddress because it does byte order stuff and we don't need it.
		SetFamily( FAMILY_INET6 );
		uint8_t *haddress = reinterpret_cast<sockaddr_in6 *>( host_address )->sin6_addr.s6_addr;
		uint8_t *raddress = reinterpret_cast<sockaddr_in6 *>( result->ai_addr )->sin6_addr.s6_addr;
		std::memcpy( haddress, raddress, 16 );

		freeaddrinfo( result );
		result = NULL;
		return true;
	}

	return false;
}

} // namespace MultiLibrary
