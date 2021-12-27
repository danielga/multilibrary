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

#include <MultiLibrary/Common/Export.hpp>
#include <locale>

namespace MultiLibrary
{

/*!
 \brief UTF-8 related functions such as decoding and length calculation.
 */
namespace UTF8
{

/*!
 \brief Decode a single UTF-8 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param output Output Unicode codepoint.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator pointing to one past the last read element of the input sequence.
 */
template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

/*!
 \brief Encode a single UTF-8 codepoint.

 \param input Unicode codepoint to encode.
 \param output Iterator to the begining of the output.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Output>
static Output Encode( uint32_t input, Output output, uint8_t replace = 0 );

/*!
 \brief Advance to the next UTF-8 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Iterator to the codepoint following the begining of the input.
 */
template<typename Input>
static Input Next( Input begin, Input end );

/*!
 \brief Calculate the length of the UTF-8 sequence.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Length of the UTF-8 sequence.
 */
template<typename Input>
static size_t Length( Input begin, Input end );

/*!
 \brief Decode ANSI input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param loc (optional) Locale of the input sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromANSI( Input begin, Input end, Output out, const std::locale &loc = std::locale( ) );

/*!
 \brief Decode wide string input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-16 input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF16( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-32 input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF32( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-8 input into ANSI.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to ANSI.
 \param loc (optional) Locale of the output sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToANSI( Input begin, Input end, Output out, char replace = 0, const std::locale &loc = std::locale( ) );

/*!
 \brief Encode UTF-8 input into wide string.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to wide string.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

/*!
 \brief Encode UTF-8 input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF16( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-8 input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF32( Input begin, Input end, Output out );

} // namespace UTF8

/*!
 \brief UTF-16 related functions such as decoding and length calculation.
 */
namespace UTF16
{

/*!
 \brief Decode a single UTF-16 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param output Output Unicode codepoint.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator pointing to one past the last read element of the input sequence.
 */
template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

/*!
 \brief Encode a single UTF-16 codepoint.

 \param input Unicode codepoint to encode.
 \param output Iterator to the begining of the output.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Output>
static Output Encode( uint32_t input, Output output, uint16_t replace = 0 );

/*!
 \brief Advance to the next UTF-16 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Iterator to the codepoint following the begining of the input.
 */
template<typename Input>
static Input Next( Input begin, Input end );

/*!
 \brief Calculate the length of the UTF-16 sequence.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Length of the UTF-16 sequence.
 */
template<typename Input>
static size_t Length( Input begin, Input end );

/*!
 \brief Decode ANSI input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param loc (optional) Locale of the input sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromANSI( Input begin, Input end, Output out, const std::locale &loc = std::locale( ) );

/*!
 \brief Decode wide string input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-8 input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF8( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-32 input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF32( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-16 input into ANSI.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to ANSI.
 \param loc (optional) Locale of the output sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToANSI( Input begin, Input end, Output out, char replace = 0, const std::locale &loc = std::locale( ) );

/*!
 \brief Encode UTF-16 input into wide string.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to wide string.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

/*!
 \brief Encode UTF-16 input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF8( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-16 input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF32( Input begin, Input end, Output out );

} // namespace UTF16

/*!
 \brief UTF-32 related functions such as decoding and length calculation.
 */
namespace UTF32
{

/*!
 \brief Decode a single UTF-32 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param output Output Unicode codepoint.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator pointing to one past the last read element of the input sequence.
 */
template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

/*!
 \brief Encode a single UTF-32 codepoint.

 \param input Unicode codepoint to encode.
 \param output Iterator to the begining of the output.
 \param replace (optional) Replacement value for invalid Unicode codepoint.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Output>
static Output Encode( uint32_t input, Output output, uint32_t replace = 0 );

/*!
 \brief Advance to the next UTF-32 codepoint.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Iterator to the codepoint following the begining of the input.
 */
template<typename Input>
static Input Next( Input begin, Input end );

/*!
 \brief Calculate the length of the UTF-32 sequence.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.

 \return Length of the UTF-32 sequence.
 */
template<typename Input>
static size_t Length( Input begin, Input end );

/*!
 \brief Decode ANSI input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param loc (optional) Locale of the input sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromANSI( Input begin, Input end, Output out, const std::locale &loc = std::locale( ) );

/*!
 \brief Decode wide string input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-8 input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF8( Input begin, Input end, Output out );

/*!
 \brief Decode UTF-16 input into UTF-32.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output FromUTF16( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-32 input into ANSI.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to ANSI.
 \param loc (optional) Locale of the output sequence.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToANSI( Input begin, Input end, Output out, char replace = 0, const std::locale &loc = std::locale( ) );

/*!
 \brief Encode UTF-32 input into wide string.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.
 \param replace (optional) Replacement for codepoints not convertible to wide string.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

/*!
 \brief Encode UTF-32 input into UTF-8.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF8( Input begin, Input end, Output out );

/*!
 \brief Encode UTF-32 input into UTF-16.

 \param begin Iterator to the begining of the input.
 \param end Iterator to the end of the input.
 \param out Iterator to the begining of the output.

 \return Iterator to the end of the output sequence which was written.
 */
template<typename Input, typename Output>
static Output ToUTF16( Input begin, Input end, Output out );

} // namespace UTF32

#include <MultiLibrary/Common/Unicode.inl>

} // namespace MultiLibrary
