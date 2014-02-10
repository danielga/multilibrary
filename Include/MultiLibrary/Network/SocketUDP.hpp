#pragma once

#include <MultiLibrary/Network/Socket.hpp>
#include <MultiLibrary/Network/SocketError.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_NETWORK_API SocketUDP : public Socket
{
public:
	SocketUDP( IPAddressFamily family = FAMILY_INET );

	virtual SocketError Listen( int max_con_requests );

	virtual bool Open( );

	virtual SocketType Type( ) const;
};

} // namespace MultiLibrary