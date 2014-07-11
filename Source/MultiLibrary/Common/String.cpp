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

#include <MultiLibrary/Common/String.hpp>

namespace MultiLibrary
{

String::String( )
{ }

String::String( const char *str )
{
	UTF8::FromANSI( str, str + std::strlen( str ), std::back_inserter( utf8_string ) );
}

String::String( const wchar_t *str )
{
	UTF8::FromWideString( str, str + std::wcslen( str ), std::back_inserter( utf8_string ) );
}

String::String( const std::string &str )
{
	UTF8::FromANSI( str.begin( ), str.end( ), std::back_inserter( utf8_string ) );
}

String::String( const std::wstring &str )
{
	UTF8::FromWideString( str.begin( ), str.end( ), std::back_inserter( utf8_string ) );
}

String::String( const String &str ) :
	utf8_string( str.utf8_string )
{ }

String &String::operator=( const String &str )
{
	utf8_string = str.utf8_string;
	return *this;
}

String &String::operator+=( const String &str )
{
	utf8_string += str.utf8_string;
	return *this;
}

std::string String::ToANSI( ) const
{
	std::string str;
	UTF8::ToANSI( utf8_string.begin( ), utf8_string.end( ), std::back_inserter( str ) );
	return str;
}

std::wstring String::ToWideString( ) const
{
	std::wstring str;
	UTF8::ToWideString( utf8_string.begin( ), utf8_string.end( ), std::back_inserter( str ) );
	return str;
}

std::string String::ToUTF8( ) const
{
	return utf8_string;
}

std::basic_string<uint16_t> String::ToUTF16( ) const
{
	std::basic_string<uint16_t> str;
	UTF8::ToUTF16( utf8_string.begin( ), utf8_string.end( ), std::back_inserter( str ) );
	return str;
}

std::basic_string<uint32_t> String::ToUTF32( ) const
{
	std::basic_string<uint32_t> str;
	UTF8::ToUTF32( utf8_string.begin( ), utf8_string.end( ), std::back_inserter( str ) );
	return str;
}

String::operator std::string( ) const
{
	return ToANSI( );
}

String::operator std::wstring( ) const
{
	return ToWideString( );
}

// Written by Jack Handy - jakkhandy@hotmail.com
// Adapted for C++ (MultiLibrary)
bool String::WildcardCompare( const std::string &str, const std::string &wildcard )
{
	std::string::const_iterator string = str.begin( ), strend = str.end( ), wild = wildcard.begin( ), wildend = wildcard.end( ), cp, mp;

	while( string != strend && *wild != '*' )
	{
		if( *wild != *string && *wild != '?' )
			return false;

		++wild;
		++string;
	}

	while( string != strend )
	{
		if( *wild == '*' )
		{
			if( ++wild == wildend )
				return true;

			mp = wild;
			cp = string + 1;
		}
		else if( *wild == *string || *wild == '?' )
		{
			++wild;
			++string;
		}
		else
		{
			wild = mp;
			string = cp++;
		}
	}

	while( *wild == '*' )
		++wild;

	return wild == wildend;
}

} // namespace MultiLibrary