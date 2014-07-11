/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
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
 *
 *************************************************************************/

#pragma once

#include <MultiLibrary/Network/Export.hpp>
#include <MultiLibrary/Network/SocketTCP.hpp>
#include <vector>

namespace MultiLibrary
{

class MULTILIBRARY_NETWORK_API FTP
{
public:
	enum TransferMode
	{
		Binary,
		Ascii,
		Ebcdic
	};

	class MULTILIBRARY_NETWORK_API Response
	{
	public:
		enum Status
		{
			InvalidFile = -4,
			ConnectionClosed = -2,
			InvalidResponse = -3,
			ConnectionFailed = -1,

			RestartMarkerReply = 110,
			ServiceReadySoon = 120,
			DataConnectionAlreadyOpened = 125,
			OpeningDataConnection = 150,

			Ok = 200,
			PointlessCommand = 202,
			SystemStatus = 211,
			DirectoryStatus = 212,
			FileStatus = 213,
			HelpMessage = 214,
			SystemType = 215,
			ServiceReady = 220,
			ClosingConnection = 221,
			DataConnectionOpened = 225,
			ClosingDataConnection = 226,
			EnteringPassiveMode = 227,
			LoggedIn = 230,
			FileActionOk = 250,
			DirectoryOk = 257,

			NeedPassword = 331,
			NeedAccountToLogIn = 332,
			NeedInformation = 350,

			ServiceUnavailable = 421,
			DataConnectionUnavailable = 425,
			TransferAborted = 426,
			FileActionAborted = 450,
			LocalError = 451,
			InsufficientStorageSpace = 452,

			CommandUnknown = 500,
			ParametersUnknown = 501,
			CommandNotImplemented = 502,
			BadCommandSequence = 503,
			ParameterNotImplemented = 504,
			NotLoggedIn = 530,
			NeedAccountToStore = 532,
			FileUnavailable = 550,
			PageTypeUnknown = 551,
			NotEnoughMemory = 552,
			FilenameNotAllowed = 553
		};

		explicit Response( Status code = InvalidResponse, const std::string &message = "" );

		bool IsOK( ) const;
		Status GetStatus( ) const;
		const std::string &GetMessage( ) const;

	private:
		Status status;
		std::string message;
	};

	class MULTILIBRARY_NETWORK_API DirectoryResponse : public Response
	{
	public:
		DirectoryResponse( const Response &response );

		const std::string &GetDirectory( ) const;

	private:
		std::string directory;
	};

	class MULTILIBRARY_NETWORK_API ListingResponse : public Response
	{
	public:
		ListingResponse( const Response &response, const std::vector<char> &data );

		const std::vector<std::string> &GetListing( ) const;

	private :
		std::vector<std::string> listing;
	};

	~FTP( );

	Response Connect( const IPAddress &server, uint32_t timeout = 0 );
	Response Disconnect( );

	Response Login( );
	Response Login( const std::string &name, const std::string &password );

	Response KeepAlive( );

	DirectoryResponse GetWorkingDirectory( );
	Response ChangeDirectory( const std::string &directory );

	ListingResponse GetDirectoryListing( const std::string &directory = "" );
	Response CreateDirectory( const std::string &name );
	Response DeleteDirectory( const std::string &name );

	Response RenameFile( const std::string &file, const std::string &newName );
	Response DeleteFile( const std::string &name );

	Response Download( const std::string &remoteFile, const std::string &localPath, TransferMode mode = Binary );

	Response Upload( const std::string &localFile, const std::string &remotePath, TransferMode mode = Binary );

private:
	Response SendCommand( const std::string &command, const std::string &parameter = "" );
	Response GetResponse( );

	SocketTCP connection_socket;

	class DataChannel;
	friend DataChannel;
};

} // namespace MultiLibrary