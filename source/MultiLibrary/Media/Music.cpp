/*************************************************************************
 * MultiLibrary - http://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2017, Daniel Almeida
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

#include <MultiLibrary/Media/Music.hpp>

namespace MultiLibrary
{

Music::Music( )
{
	media_decoder.IgnoreStream( MediaDecoder::STREAMTYPE_VIDEO, true );
}

Music::~Music( )
{
	media_decoder.Close( );
}

bool Music::Open( const std::string &filename )
{
	media_decoder.Close( );

	if( media_decoder.Open( filename ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		std::chrono::microseconds duration = media_decoder.GetAudioDuration( );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::Open( const void *data, size_t data_size )
{
	media_decoder.Close( );

	if( media_decoder.Open( data, static_cast<int64_t>( data_size ) ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		std::chrono::microseconds duration = media_decoder.GetAudioDuration( );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::Open( InputStream &stream )
{
	media_decoder.Close( );

	if( media_decoder.Open( stream ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		std::chrono::microseconds duration = media_decoder.GetAudioDuration( );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::GetData( std::vector<int16_t> &buffer, size_t min )
{
	AudioFrame audio_frame;
	while( buffer.size( ) < min && media_decoder.ReadAudio( audio_frame ) )
		buffer.insert( buffer.end( ), audio_frame.data.begin( ), audio_frame.data.end( ) );

	return buffer.size( ) >= min;
}

bool Music::Seek( const std::chrono::microseconds &time_offset )
{
	return media_decoder.Seek( time_offset.count( ) );
}

} // namespace MultiLibrary
