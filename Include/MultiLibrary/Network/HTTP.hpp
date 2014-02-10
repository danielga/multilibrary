#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <MultiLibrary/Network/IPAddress.hpp>
#include <string>
#include <map>

namespace MultiLibrary
{

class MULTILIBRARY_NETWORK_API HTTP
{
public:
	class MULTILIBRARY_NETWORK_API Request
	{
	public:
		enum Method
		{
			Get,
			Post,
			Head
		};

		Request( const std::string &uri = "/", Method method = Get, const std::string &body = "" );

		void SetURI( const std::string &uri );
		void SetMethod( Method method );
		void SetHTTPVersion( uint32_t major, uint32_t minor );
		void SetBody( const std::string &body );
		void SetField( const std::string &key, const std::string &value );

		bool HasField( const std::string &key ) const;

		std::string BuildRequest( ) const;

	private:
		friend HTTP;

		std::map<std::string, std::string> fields;
		std::string uri;
		std::string body;
		Method method;
		uint32_t minorVersion;
		uint32_t majorVersion;
	};

	class MULTILIBRARY_NETWORK_API Response
	{
	public:
		enum Status
		{
			InvalidResponse = -2,
			ConnectionFailed = -1,

			Ok = 200,
			Created = 201,
			Accepted = 202,
			NoContent = 204,
			ResetContent = 205,
			PartialContent = 206,

			MultipleChoices = 300,
			MovedPermanently = 301,
			MovedTemporarily = 302,
			NotModified = 304,

			BadRequest = 400,
			Unauthorized = 401,
			Forbidden = 403,
			NotFound = 404,
			RangeNotSatisfiable = 407,

			InternalServerError = 500,
			NotImplemented = 501,
			BadGateway = 502,
			ServiceNotAvailable = 503,
			GatewayTimeout = 504,
			VersionNotSupported = 505
		};

		Response( );

		Status GetStatus( ) const;
		uint32_t GetHTTPMajorVersion( ) const;
		uint32_t GetHTTPMinorVersion( ) const;
		const std::string &GetBody( ) const;
		const std::string &GetField( const std::string &key ) const;

		void ParseResponse( const std::string &data );

	private:
		std::map<std::string, std::string> fields;
		Status status;
		std::string body;
		uint32_t minorVersion;
		uint32_t majorVersion;
	};

	HTTP( );
	HTTP( const std::string &host, uint16_t port );
	virtual ~HTTP( );

	void SetHost( const std::string &host, uint16_t port );
	// timeout is in milliseconds
	Response SendRequest( const Request &request, uint32_t timeout = 0 );

private:
	IPAddress host_address;
	std::string host_name;
};

} // namespace MultiLibrary