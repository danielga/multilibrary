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

#include <MultiLibrary/Window/Monitor.hpp>
#include <MultiLibrary/Common/Unicode.hpp>

namespace MultiLibrary
{

struct Monitor::Handle
{
	std::string name;
	std::wstring widename;
	Vector2i max_resolution;
};

Monitor::Monitor( bool use_primary_monitor ) :
	handle( nullptr )
{
	if( use_primary_monitor )
	{
		std::vector<Monitor> monitors = GetMonitors( );
		if( monitors.size( ) > 0 )
			handle.swap( monitors[0].handle );
	}
}

bool Monitor::IsValid( ) const
{
	return static_cast<bool>( handle );
}

std::string Monitor::GetName( ) const
{
	if( !IsValid( ) )
		return std::string( );

	return handle->name;
}

Vector2i Monitor::GetPhysicalSize( ) const
{
	if( !IsValid( ) )
		return Vector2i( );

	return handle->max_resolution;
}

Vector2i Monitor::GetPosition( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;


	return pos;
}

VideoMode Monitor::GetDesktopMode( ) const
{
	VideoMode mode;
	if( !IsValid( ) )
		return mode;


	return mode;
}

std::vector<GammaColors> Monitor::GetGammaRamp( ) const
{
	std::vector<GammaColors> ramp;
	if( !IsValid( ) )
		return ramp;


	return ramp;
}

void Monitor::SetGamma( float gamma )
{
	if( !IsValid( ) || gamma <= 0.0f )
		return;

	std::vector<GammaColors> gamma_ramp;
	for( size_t i = 0; i < 256; ++i )
	{
		double value = pow( i / 255.0, 1.0 / gamma ) * 65535.0 + 0.5;
		if( value > 65535.0 )
			value = 65535.0;

		uint16_t uvalue = static_cast<uint16_t>( value );
		gamma_ramp.push_back( GammaColors( uvalue, uvalue, uvalue ) );
	}

	return SetGammaRamp( gamma_ramp );
}

void Monitor::SetGammaRamp( const std::vector<GammaColors> &gamma_ramp )
{
	if( !IsValid( ) || gamma_ramp.size( ) != 256 )
		return;


}

std::vector<VideoMode> Monitor::GetFullscreenModes( ) const
{
	std::vector<VideoMode> video_modes;
	if( !IsValid( ) )
		return video_modes;


	return video_modes;
}

const Monitor::Handle &Monitor::GetHandle( ) const
{
	return *handle;
}

std::vector<Monitor> Monitor::GetMonitors( )
{
	std::vector<Monitor> monitors;



	return monitors;
}

} // namespace MultiLibrary
