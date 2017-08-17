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

#include <MultiLibrary/Media/Listener.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

void Listener::SetMasterGain( float gain )
{
	alCheck( alListenerf( AL_GAIN, gain ) );
}

float Listener::GetMasterGain( )
{
	float gain = 0.0f;
	alCheck( alGetListenerf( AL_GAIN, &gain ) );
	return gain;
}

void Listener::SetPosition( const Vector3f &pos )
{
	alCheck( alListenerfv( AL_POSITION, reinterpret_cast<const ALfloat *>( &pos ) ) );
}

Vector3f Listener::GetPosition( )
{
	Vector3f pos;
	alCheck( alGetListenerfv( AL_POSITION, reinterpret_cast<ALfloat *>( &pos ) ) );
	return pos;
}

void Listener::SetVelocity( const Vector3f &vel )
{
	alCheck( alListenerfv( AL_VELOCITY, reinterpret_cast<const ALfloat *>( &vel ) ) );
}

Vector3f Listener::GetVelocity( )
{
	Vector3f vel;
	alCheck( alGetListenerfv( AL_VELOCITY, reinterpret_cast<ALfloat *>( &vel ) ) );
	return vel;
}

void Listener::SetDirection( const Vector3f &dir )
{
	float orientation[] = { dir.x, dir.y, dir.z, 0.0f, 1.0f, 0.0f };
	alCheck( alListenerfv( AL_ORIENTATION, orientation ) );
}

Vector3f Listener::GetDirection( )
{
	float orientation[6];
	alCheck( alGetListenerfv( AL_ORIENTATION, orientation ) );
	return Vector3f( orientation[0], orientation[1], orientation[2] );
}

} // namespace MultiLibrary
