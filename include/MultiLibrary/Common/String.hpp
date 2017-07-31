/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015-2017, Daniel Almeida
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

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <string>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API String
{
public:
	String( );
	explicit String( const char *str );
	explicit String( const wchar_t *str );
	explicit String( const std::string &str );
	explicit String( const std::wstring &str );
	explicit String( const String &str );

	String &operator=( const String &str );
	String &operator+=( const String &str );

	template<typename Input> static String FromANSI( Input begin, Input end );
	template<typename Input> static String FromWideString( Input begin, Input end );
	template<typename Input> static String FromUTF8( Input begin, Input end );
	template<typename Input> static String FromUTF16( Input begin, Input end );
	template<typename Input> static String FromUTF32( Input begin, Input end );

	std::string ToANSI( ) const;
	std::wstring ToWideString( ) const;
	std::string ToUTF8( ) const;
	std::basic_string<uint16_t> ToUTF16( ) const;
	std::basic_string<uint32_t> ToUTF32( ) const;

	operator std::string( ) const;
	operator std::wstring( ) const;

	// Written by Jack Handy - jakkhandy@hotmail.com
	// Adapted for C++ (MultiLibrary)
	static bool WildcardCompare( const std::string &str, const std::string &wildcard );

private:
	std::string utf8_string;
};

#include <MultiLibrary/Common/String.inl>

} // namespace MultiLibrary
