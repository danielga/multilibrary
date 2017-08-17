/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
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

#include <MultiLibrary/Media/Sound.hpp>
#include <MultiLibrary/Media/SoundBuffer.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

Sound::Sound( ) :
	sound_buffer( nullptr )
{ }

Sound::~Sound( )
{
	Stop( );
}

void Sound::Play( )
{
	alCheck( alSourcePlay( audio_source ) );
}

void Sound::Pause( )
{
	alCheck( alSourcePause( audio_source ) );
}

void Sound::Stop( )
{
	alCheck( alSourceStop( audio_source ) );
}

void Sound::SetBuffer( SoundBuffer &soundbuffer )
{
	sound_buffer = &soundbuffer;
	alCheck( alSourcei( audio_source, AL_BUFFER, soundbuffer.GetBufferIndex( ) ) );
}

const SoundBuffer &Sound::GetBuffer( )
{
	return *sound_buffer;
}

void Sound::SetLoop( bool loop )
{
	alCheck( alSourcei( audio_source, AL_LOOPING, loop ) );
}

bool Sound::GetLoop( )
{
	ALint loop;
	alCheck( alGetSourcei( audio_source, AL_LOOPING, &loop ) );
	return loop != 0;
}

void Sound::SetPlayingOffset( const std::chrono::microseconds &timeOffset )
{
	alCheck( alSourcef( audio_source, AL_SEC_OFFSET, timeOffset.count( ) / 1000000.0f ) );
}

std::chrono::microseconds Sound::GetPlayingOffset( )
{
	ALfloat secs = 0.0f;
	alCheck( alGetSourcef( audio_source, AL_SEC_OFFSET, &secs ) );
	return std::chrono::microseconds( static_cast<int64_t>( secs * 1000000 ) );
}

void Sound::ResetPublisher( )
{
	if( sound_buffer != nullptr )
	{
		Stop( );
		alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
		sound_buffer = nullptr;
	}
}

} // namespace MultiLibrary
