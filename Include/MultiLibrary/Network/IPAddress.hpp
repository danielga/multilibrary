#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <string>

struct sockaddr;
struct sockaddr_storage;
struct sockaddr_in;
struct sockaddr_in6;

namespace MultiLibrary
{

enum IPAddressFamily
{
	FAMILY_INET = 2,
	FAMILY_INET6 = 23
};

class MULTILIBRARY_NETWORK_API IPAddress
{
public:
	IPAddress( IPAddressFamily family = FAMILY_INET );

	IPAddress( const std::string &address, uint16_t port );

	IPAddress( const sockaddr_in &address );
	IPAddress( uint32_t address, uint16_t port );
	IPAddress( const uint8_t address[4], uint16_t port );

	IPAddress( const sockaddr_in6 &address );
	IPAddress( const uint16_t address[8], uint16_t port );

	virtual ~IPAddress( );

	IPAddress &operator=( const IPAddress &address );

	void SetIPAddress( const sockaddr_in &address );
	void SetIPAddress( const sockaddr_in6 &address );

	void SetAddress( const std::string &address );

	void SetAddress( uint32_t address );
	void SetAddress( const uint8_t address[4] );

	void SetAddress( const uint16_t address[8] );

	void SetPort( uint16_t port );

	uint32_t GetAddress( ) const;
	void GetAddress( uint8_t address[4] ) const;

	void GetAddress( uint16_t address[8] ) const;

	uint16_t GetPort( ) const;

	const sockaddr *ToSocketAddress( ) const;
	sockaddr *GetSocketAddress( );
	size_t GetAddressSize( ) const;

	std::string ToString( ) const;

	void SetFamily( IPAddressFamily family );
	IPAddressFamily GetFamily( ) const;

protected:
	void InitializeAddress( );
	bool ResolveString( const std::string &address );

	sockaddr_storage *host_address;
};

} // namespace MultiLibrary