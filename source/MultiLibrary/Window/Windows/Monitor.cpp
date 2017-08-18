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

#include <MultiLibrary/Window/Windows/Monitor.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <utility>
#include <iterator>
#include <Windows.h>

#if !defined EDS_ROTATEDMODE

	#define EDS_ROTATEDMODE 0x00000004

#endif

namespace MultiLibrary
{

namespace Internal
{

static void SplitBitsPerPixel( int32_t bpp, int32_t &red_bits, int32_t &green_bits, int32_t &blue_bits )
{
	if( bpp == 32 )
		bpp = 24;

	red_bits = green_bits = blue_bits = bpp / 3;

	int32_t delta = bpp - red_bits * 3;
	if( delta >= 1 )
		green_bits += 1;

	if( delta == 2 )
		red_bits += 1;
}

}

Monitor::Handle::Handle( HDC handle, const std::wstring &wname, const std::wstring &devwname, int32_t w, int32_t h ) :
	device( handle, DeleteDC ),
	wname( wname ),
	device_wname( devwname ),
	max_resolution( w, h )
{
	UTF8::FromUTF16( wname.begin( ), wname.end( ), std::back_inserter( name ) );
	UTF8::FromUTF16( devwname.begin( ), devwname.end( ), std::back_inserter( device_name ) );
}

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

	DEVMODE settings;
	ZeroMemory( &settings, sizeof( settings ) );
	settings.dmSize = sizeof( settings );
	if( EnumDisplaySettingsEx( handle->device_wname.c_str( ), ENUM_CURRENT_SETTINGS, &settings, EDS_ROTATEDMODE ) == FALSE )
		return pos;

	pos.x = settings.dmPosition.x;
	pos.y = settings.dmPosition.y;
	return pos;
}

VideoMode Monitor::GetDesktopMode( ) const
{
	VideoMode mode;
	if( !IsValid( ) )
		return mode;

	DEVMODE dm;
	ZeroMemory( &dm, sizeof( dm ) );
	dm.dmSize = sizeof( dm );
	if( EnumDisplaySettings( handle->device_wname.c_str( ), ENUM_CURRENT_SETTINGS, &dm ) == FALSE )
		return mode;

	mode.width  = dm.dmPelsWidth;
	mode.height = dm.dmPelsHeight;
	mode.refresh_rate = dm.dmDisplayFrequency;
	Internal::SplitBitsPerPixel( dm.dmBitsPerPel, mode.red_bits, mode.green_bits, mode.blue_bits );

	return mode;
}

std::vector<GammaColors> Monitor::GetGammaRamp( ) const
{
	std::vector<GammaColors> ramp;
	if( !IsValid( ) )
		return ramp;

	WORD values[768];
	GetDeviceGammaRamp( handle->device.get( ), values );

	WORD *reds = values, *greens = values + 256, *blues = values + 512;
	for( size_t i = 0; i < 256; ++i )
		ramp.push_back( GammaColors( *reds++, *greens++, *blues++ ) );

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

	WORD values[768];
	WORD *reds = values, *greens = values + 256, *blues = values + 512;
	for( std::vector<GammaColors>::const_iterator it = gamma_ramp.begin( ); it != gamma_ramp.end( ); ++it )
	{
		const GammaColors &gamma = *it;
		*reds++ = gamma.red;
		*greens++ = gamma.green;
		*blues++ = gamma.blue;
	}

	SetDeviceGammaRamp( handle->device.get( ), values );
}

std::vector<VideoMode> Monitor::GetFullscreenModes( ) const
{
	std::vector<VideoMode> video_modes;
	if( !IsValid( ) )
		return video_modes;

	DEVMODE dm;
	for( DWORD mode_index = 0; ; ++mode_index )
	{
		ZeroMemory( &dm, sizeof( dm ) );
		dm.dmSize = sizeof( dm );

		if( EnumDisplaySettings( handle->device_wname.c_str( ), mode_index, &dm ) == FALSE )
			break;

		if( dm.dmBitsPerPel < 15 )
			continue;

		VideoMode mode;
		mode.width  = dm.dmPelsWidth;
		mode.height = dm.dmPelsHeight;
		mode.refresh_rate = dm.dmDisplayFrequency;
		Internal::SplitBitsPerPixel( dm.dmBitsPerPel, mode.red_bits, mode.green_bits, mode.blue_bits );

		video_modes.push_back( mode );
	}

	return video_modes;
}

const Monitor::Handle &Monitor::GetHandle( ) const
{
	return *handle;
}

std::vector<Monitor> Monitor::GetMonitors( )
{
	DISPLAY_DEVICE adapter;
	DISPLAY_DEVICE display;
	std::vector<Monitor> monitors;
	for( DWORD adapter_index = 0; ; ++adapter_index )
	{
		ZeroMemory( &adapter, sizeof( adapter ) );
		adapter.cb = sizeof( adapter );
		if( EnumDisplayDevices( nullptr, adapter_index, &adapter, 0 ) == FALSE )
			break;

		if( adapter.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER || !( adapter.StateFlags & DISPLAY_DEVICE_ACTIVE ) )
			continue;

		for( DWORD monitor_index = 0; ; ++monitor_index )
		{
			ZeroMemory( &display, sizeof( display ) );
			display.cb = sizeof( display );
			if( EnumDisplayDevices( adapter.DeviceName, monitor_index, &display, 0 ) == FALSE )
				break;

			HDC dc = CreateDC( L"DISPLAY", display.DeviceString, nullptr, nullptr );
			int32_t w = GetDeviceCaps( dc, HORZSIZE ), h = GetDeviceCaps( dc, VERTSIZE );

			Monitor monitor;
			monitor.handle = std::make_shared<Handle>( dc, display.DeviceString, display.DeviceName, w, h );
			if( display.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE )
				monitors.insert( monitors.begin( ), monitor );
			else
				monitors.push_back( monitor );
		}
	}

	return monitors;
}

} // namespace MultiLibrary
