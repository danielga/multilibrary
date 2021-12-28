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

#include <MultiLibrary/Common/InputStream.hpp>

namespace MultiLibrary
{

InputStream &InputStream::operator>>( std::string &data )
{
	char ch = '\0', nch = '\0';
	if( Read( &ch, sizeof( ch ) ) != sizeof( ch ) || ch == '\0' || ch == '\n' )
		return *this;

	do
	{
		if( Read( &nch, sizeof( nch ) ) != sizeof( nch ) || nch == '\0' || nch == '\n' )
		{

#ifdef _WIN32

			if( ch == '\r' )
				break;

#endif

			data += ch;
			break;
		}

		data += ch;

		ch = nch;
	}
	while( ch != '\0' && ch != '\n' );

	return *this;
}

InputStream &InputStream::operator>>( std::wstring &data )
{
	wchar_t ch = L'\0', nch = L'\0';
	if( Read( &ch, sizeof( ch ) ) != sizeof( ch ) || ch == L'\0' || ch == L'\n' )
		return *this;

	do
	{
		if( Read( &nch, sizeof( nch ) ) != sizeof( nch ) || nch == L'\0' || nch == L'\n' )
		{

#ifdef _WIN32

			if( ch == L'\r' )
				break;

#endif

			data += ch;
			break;
		}

		data += ch;

		ch = nch;
	}
	while( ch != L'\0' && ch != L'\n' );

	return *this;
}

} // namespace MultiLibrary
