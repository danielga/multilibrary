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

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Window/VideoMode.hpp>
#include <MultiLibrary/Common/Vector2.hpp>
#include <string>
#include <vector>

namespace MultiLibrary
{

struct MonitorHandle;

struct GammaColors
{
	GammaColors( ) :
		red( 0 ),
		green( 0 ),
		blue( 0 )
	{ }

	GammaColors( uint16_t red, uint16_t blue, uint16_t green ) :
		red( red ),
		green( green ),
		blue( blue )
	{ }

	uint16_t red;
	uint16_t green;
	uint16_t blue;
};

class MULTILIBRARY_WINDOW_API Monitor
{
public:
	explicit Monitor( bool use_primary_monitor = false );
	~Monitor( );

	bool IsValid( ) const;

	std::string GetName( ) const;

	Vector2i GetPhysicalSize( ) const;

	Vector2i GetPosition( ) const;

	VideoMode GetDesktopMode( ) const;

	std::vector<GammaColors> GetGammaRamp( ) const;
	void SetGamma( float gamma );
	void SetGammaRamp( const std::vector<GammaColors> &gamma_ramp );

	std::vector<VideoMode> GetFullscreenModes( ) const;

	MonitorHandle *GetHandle( ) const;

	static std::vector<Monitor> GetMonitors( );

private:
	Monitor( MonitorHandle *handle );

	MonitorHandle *monitor_internal;
};

} // namespace MultiLibrary