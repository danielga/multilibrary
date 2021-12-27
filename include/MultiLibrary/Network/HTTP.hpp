/*************************************************************************
 * MultiLibrary - https://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2022, Daniel Almeida
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

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
		std::map<std::string, std::string> fields;
		std::string uri;
		std::string body;
		Method method;
		uint32_t minorVersion;
		uint32_t majorVersion;

		friend HTTP;
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
