#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <MultiLibrary/Network/Socket.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_NETWORK_API SocketTCP : public Socket
{
public:
	SocketTCP( IPAddressFamily family = FAMILY_INET );

	virtual bool Open( );

	virtual SocketType Type( ) const;
};

} // namespace MultiLibrary