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

#include <MultiLibrary/Media/SoundStream.hpp>
#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

SoundStream::SoundStream( ) :
	thread_active( true ),
	stream_thread( &SoundStream::DataStreamer, this ),
	is_streaming( false ),
	should_loop( false ),
	samples_processed( 0 ),
	channel_count( 0 ),
	sample_rate( 0 ),
	audio_format( 0 ),
	minimum_bufsize( 0 )
{ }

SoundStream::~SoundStream( )
{
	Stop( );

	thread_active = false;
	stream_thread.join( );
}

void SoundStream::Play( )
{
	if( is_streaming )
	{
		alCheck( alSourcePlay( audio_source ) );
		return;
	}

	Seek( std::chrono::milliseconds::zero( ) );

	samples_processed = 0;
	is_streaming = true;
}

void SoundStream::Pause( )
{
	alCheck( alSourcePause( audio_source ) );
}

void SoundStream::Stop( )
{
	is_streaming = false;
}

unsigned int SoundStream::GetChannelCount( ) const
{
	return channel_count;
}

unsigned int SoundStream::GetSampleRate( ) const
{
	return sample_rate;
}

std::chrono::microseconds SoundStream::GetDuration( ) const
{
	return audio_duration;
}

SoundStatus SoundStream::GetStatus( ) const
{
	SoundStatus status = SoundSource::GetStatus( );

	if( status == Stopped && is_streaming )
		status = Playing;

	return status;
}

void SoundStream::SetPlayingOffset( const std::chrono::microseconds &time_offset )
{
	Stop( );

	Seek( time_offset );

	samples_processed = static_cast<uint64_t>( time_offset.count( ) / 1000000.0 * sample_rate * channel_count );
	is_streaming = true;
}

std::chrono::microseconds SoundStream::GetPlayingOffset( ) const
{
	ALfloat secs = 0.0f;
	alCheck( alGetSourcef( audio_source, AL_SEC_OFFSET, &secs ) );
	return std::chrono::microseconds( static_cast<int64_t>( secs + static_cast<double>( samples_processed ) / sample_rate / channel_count ) );
}

void SoundStream::SetLoop( bool looping )
{
	should_loop = looping;
}

bool SoundStream::GetLoop( )
{
	return should_loop;
}

void SoundStream::Initialize( uint32_t channels, uint32_t sampleRate, const std::chrono::microseconds &duration )
{
	channel_count = channels;
	sample_rate = sampleRate;
	audio_duration = duration;
	audio_format = AudioDevice::GetFormatFromChannelCount( channels );

	minimum_bufsize = sample_rate * channel_count * sizeof( int16_t ) / 10;

	temp_buffer.resize( minimum_bufsize );
	std::vector<int16_t>( temp_buffer ).swap( temp_buffer );
	temp_buffer.resize( 0 );
}

void SoundStream::DataStreamer( )
{
	std::chrono::milliseconds millisecs = std::chrono::milliseconds( 1 );

	while( thread_active )
	{
		if( !is_streaming )
		{
			std::this_thread::sleep_for( millisecs );
			continue;
		}

		alCheck( alGenBuffers( MAX_AUDIO_BUFFERS, audio_buffers ) );

		bool requestStop = false;
		for( uint8_t i = 0; i < MAX_AUDIO_BUFFERS; ++i )
		{
			end_buffer[i] = false;
			if( !requestStop && FillAndPushBuffer( i ) )
				requestStop = true;
		}

		alCheck( alSourcePlay( audio_source ) );

		while( is_streaming )
		{
			if( SoundSource::GetStatus( ) == Stopped )
			{
				if( !requestStop )
					alCheck( alSourcePlay( audio_source ) );
				else
					is_streaming = false;
			}

			ALint nbProcessed = 0;
			alCheck( alGetSourcei( audio_source, AL_BUFFERS_PROCESSED, &nbProcessed ) );

			while( nbProcessed-- )
			{
				ALuint buffer;
				alCheck( alSourceUnqueueBuffers( audio_source, 1, &buffer ) );

				uint32_t bufferNum = 0;
				for( uint8_t i = 0; i < MAX_AUDIO_BUFFERS; ++i )
					if( audio_buffers[i] == buffer )
					{
						bufferNum = i;
						break;
					}

				if( end_buffer[bufferNum] )
				{
					samples_processed = 0;
					end_buffer[bufferNum] = false;
				}
				else
				{
					ALint size, bits;
					alCheck( alGetBufferi( buffer, AL_SIZE, &size ) );
					alCheck( alGetBufferi( buffer, AL_BITS, &bits ) );
					samples_processed += size / ( bits / 8 );
				}

				if( !requestStop )
					if( FillAndPushBuffer( bufferNum ) )
						requestStop = true;
			}

			if( SoundSource::GetStatus( ) != Stopped )
				std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
		}

		alCheck( alSourceStop( audio_source ) );

		ALint nbQueued = 0;
		alCheck( alGetSourcei( audio_source, AL_BUFFERS_QUEUED, &nbQueued ) );

		ALuint buffer = 0;
		for( ALint i = 0; i < nbQueued; ++i )
			alCheck( alSourceUnqueueBuffers( audio_source, 1, &buffer ) );

		alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
		alCheck( alDeleteBuffers( MAX_AUDIO_BUFFERS, audio_buffers ) );
	}
}

bool SoundStream::FillAndPushBuffer( uint32_t buffer_num )
{
	bool requestStop = false;

	if( !GetData( temp_buffer, minimum_bufsize ) )
	{
		end_buffer[buffer_num] = true;

		if( should_loop )
		{
			Seek( std::chrono::milliseconds::zero( ) );

			if( temp_buffer.size( ) == 0 )
				return FillAndPushBuffer( buffer_num );
		}
		else
		{
			requestStop = true;
		}
	}

	if( temp_buffer.size( ) > 0 )
	{
		uint32_t buffer = audio_buffers[buffer_num];

		ALsizei size = static_cast<ALsizei>( temp_buffer.size( ) * sizeof( int16_t ) );
		alCheck( alBufferData( buffer, audio_format, &temp_buffer[0], size, sample_rate ) );

		alCheck( alSourceQueueBuffers( audio_source, 1, &buffer ) );
	}

	temp_buffer.resize( 0 );

	return requestStop;
}

} // namespace MultiLibrary
