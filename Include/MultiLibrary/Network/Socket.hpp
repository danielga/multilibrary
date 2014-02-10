#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <MultiLibrary/Network/SocketError.hpp>
#include <MultiLibrary/Network/IPAddress.hpp>

namespace MultiLibrary
{

class ByteBuffer;

#if defined _WIN32

	typedef uintptr_t SocketHandle;

#elif defined __APPLE__ || defined __linux

	typedef int SocketHandle;
	
#endif

enum SocketType
{
	INVALID = -1,
	UDP,
	TCP
};

class MULTILIBRARY_NETWORK_API Socket
{
protected:
	Socket( IPAddressFamily family );
	virtual ~Socket( );

public:
	virtual SocketError Accept( Socket &socket, IPAddress &address );

	virtual SocketError Bind( const IPAddress &address );

	virtual bool CanRead( );
	virtual bool CanWrite( );

	virtual SocketError Close( );

	virtual SocketError Connect( const IPAddress &address );

	virtual IPAddressFamily GetAddressFamily( ) const;

	typedef void ( *unspecified_bool_type ) ( );
	static void unspecified_bool_true( ) { }

	virtual bool IsValid( ) const;
	virtual operator unspecified_bool_type( ) const;
	virtual bool operator!( ) const;

	virtual SocketError Listen( int max_con_requests );

	virtual bool Open( );

	virtual SocketError Receive( void *buffer, int size, int flags, int *received_bytes = 0 );
	virtual SocketError Receive( ByteBuffer &buffer, int flags );

	virtual SocketError ReceiveFrom( void *buffer, int size, int flags, IPAddress &address, int *received_bytes = 0 );
	virtual SocketError ReceiveFrom( ByteBuffer &buffer, int flags, IPAddress &address );

	virtual std::string RemoteAddress( ) const;
	virtual uint16_t RemotePort( ) const;
	virtual IPAddress RemoteIPAddress( ) const;

	virtual SocketError Send( const void *buffer, int size, int flags, int *sent_bytes = 0 );
	virtual SocketError Send( const ByteBuffer &buffer, int flags, int *sent_bytes = 0 );

	virtual SocketError SendTo( const void *buffer, int size, int flags, const IPAddress &address, int *sent_bytes = 0 );
	virtual SocketError SendTo( const ByteBuffer &buffer, int flags, const IPAddress &address, int *sent_bytes = 0 );

	virtual SocketError SetBlocking( bool block );
	virtual SocketError SetTimeout( uint32_t timeout );

	virtual SocketError Shutdown( int flags );

	virtual SocketType Type( ) const;

protected:
	SocketHandle socket_id;
	IPAddressFamily address_family;
	bool is_blocking;
};

} // namespace MultiLibrary