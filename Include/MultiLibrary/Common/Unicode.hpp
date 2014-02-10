#pragma once

#include <MultiLibrary/Common/Export.hpp>

namespace MultiLibrary
{

namespace UTF8
{

template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

template<typename Output>
static Output Encode( uint32_t input, Output output, uint8_t replace = 0 );

template<typename Input>
static Input Next( Input begin, Input end );

template<typename Input>
static size_t Length( Input begin, Input end );

template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF16( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF32( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

template<typename Input, typename Output>
static Output ToUTF16( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToUTF32( Input begin, Input end, Output out );

} // namespace UTF8

namespace UTF16
{

template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

template<typename Output>
static Output Encode( uint32_t input, Output output, uint16_t replace = 0 );

template<typename Input>
static Input Next( Input begin, Input end );

template<typename Input>
static size_t Length( Input begin, Input end );

template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF8( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF32( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

template<typename Input, typename Output>
static Output ToUTF8( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToUTF32( Input begin, Input end, Output out );

} // namespace UTF16

namespace UTF32
{

template<typename Input>
static Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace = 0 );

template<typename Output>
static Output Encode( uint32_t input, Output output, uint32_t replace = 0 );

template<typename Input>
static Input Next( Input begin, Input end );

template<typename Input>
static size_t Length( Input begin, Input end );

template<typename Input, typename Output>
static Output FromWideString( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF8( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output FromUTF16( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToWideString( Input begin, Input end, Output out, wchar_t replace = 0 );

template<typename Input, typename Output>
static Output ToUTF8( Input begin, Input end, Output out );

template<typename Input, typename Output>
static Output ToUTF16( Input begin, Input end, Output out );

} // namespace UTF32

#include <MultiLibrary/Common/Unicode.inl>

} // namespace MultiLibrary