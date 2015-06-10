/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015, Daniel Almeida
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
#include <chrono>
#include <string>
#include <vector>
#include <set>

namespace MultiLibrary
{

class InputStream;
class MediaDecoder;

class MULTILIBRARY_MEDIA_API SoundBuffer
{
public:
	SoundBuffer( );
	~SoundBuffer( );

	bool Load( const std::string &filename );
	bool Load( const void *data, size_t bytes );
	bool Load( InputStream &stream );
	bool Load( const int16_t *samples, size_t samples_count, uint32_t channel_count, uint32_t samplerate );

	// Allows 3D spatialization (OpenAL only allows it on mono sounds)
	bool ConvertToMono( );

	const int16_t *GetSamples( ) const;
	size_t GetSampleCount( ) const;
	uint32_t GetSampleRate( ) const;
	uint32_t GetChannelCount( ) const;
	std::chrono::microseconds GetDuration( ) const;
	uint32_t GetBufferIndex( ) const;

private:
	bool Initialize( MediaDecoder &file );
	bool Update( uint32_t channels, uint32_t samplerate );

	std::vector<short> samples_buffer;
	uint32_t buffer_id;
	std::chrono::microseconds buffer_duration;
};

} // namespace MultiLibrary