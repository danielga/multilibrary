namespace UTF8
{

template<typename Input>
Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace )
{
	static const uint32_t offsets[6] =
	{
		0x00000000, 0x00003080, 0x000E2080, 0x03C82080, 0xFA082080, 0x82082080
	};

	uint8_t ch = static_cast<uint8_t>( *begin );
	int trailingBytes = 0;
	if( ch < 192 )
		/* do nothing and break if chain */;
	else if( ch < 224 )
		trailingBytes = 1;
	else if( ch < 240 )
		trailingBytes = 2;
	else if( ch < 248 )
		trailingBytes = 3;
	else if( ch < 252 )
		trailingBytes = 4;
	else if( ch < 256 )
		trailingBytes = 5;

	if( begin + trailingBytes < end )
	{
		output = 0;
		switch( trailingBytes )
		{
		case 5:
			output += static_cast<uint8_t>( *begin );
			++begin;
			output <<= 6;

		case 4:
			output += static_cast<uint8_t>( *begin );
			++begin;
			output <<= 6;

		case 3:
			output += static_cast<uint8_t>( *begin );
			++begin;
			output <<= 6;

		case 2:
			output += static_cast<uint8_t>( *begin );
			++begin;
			output <<= 6;

		case 1:
			output += static_cast<uint8_t>( *begin );
			++begin;
			output <<= 6;

		case 0:
			output += static_cast<uint8_t>( *begin );
			++begin;
		}

		output -= offsets[trailingBytes];
	}
	else
	{
		begin = end;
		output = replace;
	}

	return begin;
}

template<typename Output>
Output Encode( uint32_t input, Output output, uint8_t replace )
{
	static const uint8_t firstBytes[7] =
	{
		0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC
	};

	if( input > 0x0010FFFF || ( input >= 0xD800 && input <= 0xDBFF ) )
	{
		if( replace )
		{
			*output = replace;
			++output;
		}
	}
	else
	{
		size_t bytestoWrite = 1;
		if( input <  0x80 )
			bytestoWrite = 1;
		else if( input <  0x800 )
			bytestoWrite = 2;
		else if( input <  0x10000 )
			bytestoWrite = 3;
		else if( input <= 0x0010FFFF )
			bytestoWrite = 4;

		uint8_t bytes[4];
		switch( bytestoWrite )
		{
		case 4:
			bytes[3] = static_cast<uint8_t>( ( input | 0x80 ) & 0xBF );
			input >>= 6;

		case 3:
			bytes[2] = static_cast<uint8_t>( ( input | 0x80 ) & 0xBF );
			input >>= 6;

		case 2:
			bytes[1] = static_cast<uint8_t>( ( input | 0x80 ) & 0xBF );
			input >>= 6;

		case 1:
			bytes[0] = static_cast<uint8_t>( input | firstBytes[bytestoWrite] );
		}

		output = std::copy( bytes, bytes + bytestoWrite, output );
	}

	return output;
}

template<typename Input>
Input Next( Input begin, Input end )
{
	uint32_t codepoint;
	return Decode( begin, end, codepoint );
}

template<typename Input>
size_t Length( Input begin, Input end )
{
	size_t length = 0;
	while( begin < end )
	{
		begin = Next( begin, end );
		++length;
	}

	return length;
}

template<typename Input, typename Output>
Output FromWideString( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		begin = UTF16::Decode( begin, end, codepoint );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		begin = UTF32::Decode( begin, end, codepoint );

#else

	#error Unknown WCHAR_MAX value

#endif
		
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF16( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF16::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF32( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF32::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToWideString( Input begin, Input end, Output out, wchar_t replace )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		out = UTF16::Encode( codepoint, out, replace );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		out = UTF32::Encode( codepoint, out, replace );

#else

	#error Unknown WCHAR_MAX value

#endif

	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF16( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF16::Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF32( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF32::Encode( codepoint, out );
	}

	return out;
}

} // namespace UTF8

namespace UTF16
{

template<typename Input>
Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace )
{
	uint16_t first = *begin;
	++begin;

	if( first >= 0xD800 && first <= 0xDBFF )
	{
		if( begin < end )
		{
			uint32_t second = *begin;
			++begin;
			if( second >= 0xDC00 && second <= 0xDFFF )
				output = ( ( first - 0xD800 ) << 10 ) + second - 0xDC00 + 0x00010000;
			else
				output = replace;
		}
		else
		{
			begin = end;
			output = replace;
		}
	}
	else
	{
		output = first;
	}

	return begin;
}

template<typename Output>
Output Encode( uint32_t input, Output output, uint16_t replace )
{
	if( input < 0xFFFF )
	{
		if( input >= 0xD800 && input <= 0xDFFF )
		{
			if( replace )
			{
				*output = replace;
				++output;
			}
		}
		else
		{
			*output = static_cast<uint16_t>( input );
			++output;
		}
	}
	else if( input > 0x0010FFFF )
	{
		if( replace )
		{
			*output = replace;
			++output;
		}
	}
	else
	{
		input -= 0x0010000;
		*output = static_cast<uint16_t>( ( input >> 10 ) + 0xD800 );
		++output;
		*output = static_cast<uint16_t>( ( input & 0x3FFUL ) + 0xDC00 );
		++output;
	}

	return output;
}

template<typename Input>
Input Next( Input begin, Input end )
{
	uint32_t codepoint;
	return Decode( begin, end, codepoint );
}

template<typename Input>
size_t Length( Input begin, Input end )
{
	size_t length = 0;
	while( begin < end )
	{
		begin = Next( begin, end );
		++length;
	}

	return length;
}

template<typename Input, typename Output>
Output FromWideString( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		begin = UTF16::Decode( begin, end, codepoint );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		begin = UTF32::Decode( begin, end, codepoint );

#else

	#error Unknown WCHAR_MAX value

#endif

		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF8( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF8::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF32( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF32::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToWideString( Input begin, Input end, Output out, wchar_t replace )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		out = UTF16::Encode( codepoint, out, replace );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		out = UTF32::Encode( codepoint, out, replace );

#else

	#error Unknown WCHAR_MAX value

#endif

	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF8( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF8::Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF32( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF32::Encode( codepoint, out );
	}

	return out;
}

} // namespace UTF16

namespace UTF32
{

template<typename Input>
Input Decode( Input begin, Input end, uint32_t &output, uint32_t replace )
{
	output = *begin;
	return ++begin;
}

template<typename Output>
Output Encode( uint32_t input, Output output, uint32_t replace )
{
	*output = input;
	return ++output;
}

template<typename Input>
Input Next( Input begin, Input end )
{
	return ++begin;
}

template<typename Input>
size_t Length( Input begin, Input end )
{
	return begin - end;
}

template<typename Input, typename Output>
Output FromWideString( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		begin = UTF16::Decode( begin, end, codepoint );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		begin = UTF32::Decode( begin, end, codepoint );

#else

	#error Unknown WCHAR_MAX value

#endif

		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF8( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF8::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output FromUTF16( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = UTF16::Decode( begin, end, codepoint );
		out = Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToWideString( Input begin, Input end, Output out, wchar_t replace )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );

#if WCHAR_MAX == 0xffff || WCHAR_MAX == 0x7fff

		out = UTF16::Encode( codepoint, out, replace );

#elif WCHAR_MAX == 0xffffffff || WCHAR_MAX == 0x7fffffff

		out = UTF32::Encode( codepoint, out, replace );

#else

	#error unknown WCHAR_MAX value

#endif

	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF8( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF8::Encode( codepoint, out );
	}

	return out;
}

template<typename Input, typename Output>
Output ToUTF16( Input begin, Input end, Output out )
{
	uint32_t codepoint;
	while( begin != end )
	{
		begin = Decode( begin, end, codepoint );
		out = UTF16::Encode( codepoint, out );
	}

	return out;
}

} // namespace UTF32