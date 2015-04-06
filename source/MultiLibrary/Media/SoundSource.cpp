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

#include <MultiLibrary/Media/SoundSource.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

SoundSource::SoundSource( )
{
	alCheck( alGenSources( 1, &audio_source ) );
	alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
}

SoundSource::~SoundSource( )
{
	alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
	alCheck( alDeleteSources( 1, &audio_source ) );
}

void SoundSource::SetPitch( float pitch )
{
	alCheck( alSourcef( audio_source, AL_PITCH, pitch ) );
}

float SoundSource::GetPitch( ) const
{
	ALfloat pitch;
	alCheck( alGetSourcef( audio_source, AL_PITCH, &pitch ) );
	return pitch;
}

void SoundSource::SetGain( float gain )
{
	alCheck( alSourcef( audio_source, AL_GAIN, gain ) );
}

float SoundSource::GetGain( ) const
{
	ALfloat gain;
	alCheck( alGetSourcef( audio_source, AL_GAIN, &gain ) );
	return gain;
}

void SoundSource::SetPosition( const Vector3f &pos )
{
	alCheck( alSourcefv( audio_source, AL_POSITION, reinterpret_cast<const ALfloat *>( &pos ) ) );
}

Vector3f SoundSource::GetPosition( )
{
	Vector3f pos;
	alCheck( alGetSourcefv( audio_source, AL_POSITION, reinterpret_cast<ALfloat *>( &pos ) ) );
	return pos;
}

void SoundSource::SetRelativeToListener( bool relative )
{
	alCheck( alSourcei( audio_source, AL_SOURCE_RELATIVE, relative ) );
}

bool SoundSource::IsRelativeToListener( ) const
{
	ALint relative;
	alCheck( alGetSourcei( audio_source, AL_SOURCE_RELATIVE, &relative ) );
	return relative != 0;
}

void SoundSource::SetMinDistance( float distance )
{
	alCheck( alSourcef( audio_source, AL_REFERENCE_DISTANCE, distance ) );
}

float SoundSource::GetMinDistance( ) const
{
	ALfloat distance;
	alCheck( alGetSourcef( audio_source, AL_REFERENCE_DISTANCE, &distance ) );
	return distance;
}

void SoundSource::SetAttenuation( float attenuation )
{
	alCheck( alSourcef( audio_source, AL_ROLLOFF_FACTOR, attenuation ) );
}

float SoundSource::GetAttenuation( ) const
{
	ALfloat attenuation;
	alCheck( alGetSourcef( audio_source, AL_ROLLOFF_FACTOR, &attenuation ) );
	return attenuation;
}

SoundStatus SoundSource::GetStatus( ) const
{
	ALint status;
	alCheck( alGetSourcei( audio_source, AL_SOURCE_STATE, &status ) );

	switch( status )
	{
		case AL_INITIAL:
		case AL_STOPPED:
			return Stopped;

		case AL_PAUSED:
			return Paused;

		case AL_PLAYING:
			return Playing;
	}

	return Stopped;
}

} // namespace MultiLibrary