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

#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

AudioDevice::AudioDevice( ) :
	openal_device( nullptr ),
	openal_context( nullptr )
{
	OpenDevice( );
}

AudioDevice::AudioDevice( const std::string &devname ) :
	openal_device( nullptr ),
	openal_context( nullptr )
{
	OpenDevice( devname );
}

AudioDevice::~AudioDevice( )
{
	CloseDevice( );
}

void AudioDevice::OpenDevice( )
{
	CloseDevice( );

	if( ( openal_device = alcOpenDevice( nullptr ) ) == nullptr )
		return;

	if( ( openal_context = alcCreateContext( openal_device, nullptr ) ) == nullptr )
		return;

	MakeCurrent( );
}

void AudioDevice::OpenDevice( const std::string &devname )
{
	CloseDevice( );

	if( ( openal_device = alcOpenDevice( devname.c_str( ) ) ) == nullptr )
		return;

	if( ( openal_context = alcCreateContext( openal_device, nullptr ) ) == nullptr )
		return;

	MakeCurrent( );
}

void AudioDevice::CloseDevice( )
{
	if( openal_context != nullptr )
	{
		if( alcGetCurrentContext( ) == openal_context )
			alcMakeContextCurrent( nullptr );

		alcDestroyContext( openal_context );
		openal_context = nullptr;
	}

	if( openal_device != nullptr )
	{
		alcCloseDevice( openal_device );
		openal_device = nullptr;
	}
}

void AudioDevice::MakeCurrent( )
{
	if( openal_context != nullptr )
		alCheck( alcMakeContextCurrent( openal_context ) );
}

std::vector<std::string> AudioDevice::GetAudioDevices( )
{
	std::vector<std::string> devicesList;
	const char *devicesNames = alcGetString( nullptr, ALC_DEVICE_SPECIFIER );
	if( devicesNames == nullptr )
		return devicesList;

	while( *devicesNames != '\0' )
	{
		std::string deviceName = devicesNames;
		devicesList.push_back( deviceName );
		devicesNames += deviceName.length( ) + 1;
	}

	return devicesList;
}

int32_t AudioDevice::GetFormatFromChannelCount( uint32_t numChannels )
{
	switch( numChannels )
	{
		case 1:
			return AL_FORMAT_MONO16;

		case 2:
			return AL_FORMAT_STEREO16;

		case 4:
			return alGetEnumValue( "AL_FORMAT_QUAD16" );

		case 6:
			return alGetEnumValue( "AL_FORMAT_51CHN16" );

		case 7:
			return alGetEnumValue( "AL_FORMAT_61CHN16" );

		case 8:
			return alGetEnumValue( "AL_FORMAT_71CHN16" );

		default:
			return 0;
	}
}

} // namespace MultiLibrary
