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

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Media/SoundSource.hpp>
#include <chrono>
#include <vector>
#include <thread>

namespace MultiLibrary
{

class MULTILIBRARY_MEDIA_API SoundStream : public SoundSource
{
public:
	SoundStream( );
	virtual ~SoundStream( );

	void Play( );
	void Pause( );
	void Stop( );

	uint32_t GetChannelCount( ) const;
	uint32_t GetSampleRate( ) const;
	std::chrono::microseconds GetDuration( ) const;

	SoundStatus GetStatus( ) const;

	void SetPlayingOffset( const std::chrono::microseconds &time_offset );
	std::chrono::microseconds GetPlayingOffset( ) const;

	void SetLoop( bool looping );
	bool GetLoop( );

protected:
	void Initialize( uint32_t channels, uint32_t sampleRate, const std::chrono::microseconds &duration );

	virtual bool GetData( std::vector<int16_t> &buffer, size_t min ) = 0;
	virtual bool Seek( const std::chrono::microseconds &time_offset ) = 0;

private:
	void DataStreamer( );
	bool FillAndPushBuffer( uint32_t buffer_num );

	static const uint8_t MAX_AUDIO_BUFFERS = 3;

	volatile bool thread_active;
	std::thread stream_thread;
	volatile bool is_streaming;
	volatile bool should_loop;
	volatile uint64_t samples_processed;

	uint32_t channel_count;
	uint32_t sample_rate;
	std::chrono::microseconds audio_duration;
	int32_t audio_format;

	uint32_t audio_buffers[MAX_AUDIO_BUFFERS];
	bool end_buffer[3];

	std::vector<int16_t> temp_buffer;
	size_t minimum_bufsize;
};

} // namespace MultiLibrary