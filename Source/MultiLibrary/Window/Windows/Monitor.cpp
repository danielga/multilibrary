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

#include <MultiLibrary/Window/Monitor.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <Windows.h>

#if !defined EDS_ROTATEDMODE

	#define EDS_ROTATEDMODE 0x00000004

#endif

namespace MultiLibrary
{

struct MonitorHandle
{
	MonitorHandle( const std::string &mname, const std::wstring &widemname, int32_t w, int32_t h ) :
		name( mname ),
		widename( widemname ),
		size( w, h )
	{ }

	MonitorHandle( const MonitorHandle &m ) :
		name( m.name ),
		widename( m.widename ),
		size( m.size )
	{ }

	std::string name;
	std::wstring widename;
	Vector2i size;
};

Monitor::Monitor( bool use_primary_monitor ) :
	monitor_internal( nullptr )
{
	if( use_primary_monitor )
	{
		std::vector<Monitor> monitors = GetMonitors( );
		if( monitors.size( ) > 0 )
			monitor_internal = new MonitorHandle( *monitors[0].monitor_internal );
	}
}

Monitor::Monitor( MonitorHandle *monitor ) :
	monitor_internal( monitor )
{ }

Monitor::~Monitor( )
{
	if( monitor_internal != nullptr )
		delete monitor_internal;
}

bool Monitor::IsValid( ) const
{
	return monitor_internal != nullptr;
}

std::string Monitor::GetName( ) const
{
	if( !IsValid( ) )
		return std::string( );

	return monitor_internal->name;
}

Vector2i Monitor::GetPhysicalSize( ) const
{
	if( !IsValid( ) )
		return Vector2i( );

	return monitor_internal->size;
}

Vector2i Monitor::GetPosition( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	DEVMODE settings;
	ZeroMemory( &settings, sizeof( settings ) );
	settings.dmSize = sizeof( settings );
	if( EnumDisplaySettingsEx( monitor_internal->widename.c_str( ), ENUM_CURRENT_SETTINGS, &settings, EDS_ROTATEDMODE ) == FALSE )
		return pos;

	pos.x = settings.dmPosition.x;
	pos.y = settings.dmPosition.y;
	return pos;
}

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

VideoMode Monitor::GetDesktopMode( ) const
{
	VideoMode mode;
	if( !IsValid( ) )
		return mode;

	DEVMODE dm;
	ZeroMemory( &dm, sizeof( dm ) );
	dm.dmSize = sizeof( dm );
	if( EnumDisplaySettings( monitor_internal->widename.c_str( ), ENUM_CURRENT_SETTINGS, &dm ) == FALSE )
		return mode;

	mode.width  = dm.dmPelsWidth;
	mode.height = dm.dmPelsHeight;
	mode.refresh_rate = dm.dmDisplayFrequency;
	SplitBitsPerPixel( dm.dmBitsPerPel, mode.red_bits, mode.green_bits, mode.blue_bits );

	return mode;
}

std::vector<GammaColors> Monitor::GetGammaRamp( ) const
{
	std::vector<GammaColors> ramp;
	if( !IsValid( ) )
		return ramp;

	DISPLAY_DEVICE display;
	ZeroMemory( &display, sizeof( display ) );
	display.cb = sizeof( display );
	if( EnumDisplayDevices( monitor_internal->widename.c_str( ), 0, &display, 0 ) == FALSE )
		return ramp;

	WORD values[768];
	HDC dc = CreateDC( L"DISPLAY", display.DeviceString, nullptr, nullptr );
	GetDeviceGammaRamp( dc, values );
	DeleteDC( dc );

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

	DISPLAY_DEVICE display;
	ZeroMemory( &display, sizeof( display ) );
	display.cb = sizeof( display );
	if( EnumDisplayDevices( monitor_internal->widename.c_str( ), 0, &display, 0 ) == FALSE )
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

	HDC dc = CreateDC( L"DISPLAY", display.DeviceString, nullptr, nullptr );
	SetDeviceGammaRamp( dc, values );
	DeleteDC( dc );
}

std::vector<VideoMode> Monitor::GetFullscreenModes( ) const
{
	std::vector<VideoMode> video_modes;
	if( !IsValid( ) )
		return video_modes;

	DEVMODE dm;
	DWORD mode_index = 0;
	for( ; ; )
	{
		ZeroMemory( &dm, sizeof( dm ) );
		dm.dmSize = sizeof( dm );

		if( EnumDisplaySettings( monitor_internal->widename.c_str( ), mode_index, &dm ) == FALSE )
			break;

		++mode_index;

		if( dm.dmBitsPerPel < 15 )
			continue;

		VideoMode mode;
		mode.width  = dm.dmPelsWidth;
		mode.height = dm.dmPelsHeight;
		mode.refresh_rate = dm.dmDisplayFrequency;
		SplitBitsPerPixel( dm.dmBitsPerPel, mode.red_bits, mode.green_bits, mode.blue_bits );

		video_modes.push_back( mode );
	}

	return video_modes;
}

MonitorHandle *Monitor::GetHandle( ) const
{
	return monitor_internal;
}

std::vector<Monitor> Monitor::GetMonitors( )
{
	DWORD adapter_index = 0;
	std::vector<Monitor> monitors;
	for( ; ; )
	{
		DISPLAY_DEVICE adapter;
		ZeroMemory( &adapter, sizeof( adapter ) );
		adapter.cb = sizeof( adapter );
		if( EnumDisplayDevices( nullptr, adapter_index, &adapter, 0 ) == FALSE )
			break;

		++adapter_index;

		if( adapter.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER || !( adapter.StateFlags & DISPLAY_DEVICE_ACTIVE ) )
			continue;

		DISPLAY_DEVICE display;
		ZeroMemory( &display, sizeof( display ) );
		display.cb = sizeof( display );
		if( EnumDisplayDevices( adapter.DeviceName, 0, &display, 0 ) == FALSE )
			continue;

		wchar_t *widename = adapter.DeviceName;
		std::string narrowname;
		UTF8::FromUTF16( widename, widename + wcslen( widename ), std::back_inserter( narrowname ) );

		HDC dc = CreateDC( L"DISPLAY", display.DeviceString, nullptr, nullptr );
		int32_t w = GetDeviceCaps( dc, HORZSIZE ), h = GetDeviceCaps( dc, VERTSIZE );
		DeleteDC( dc );

		if( adapter.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE )
			monitors.insert( monitors.begin( ), Monitor( new MonitorHandle( narrowname, widename, w, h ) ) );
		else
			monitors.push_back( Monitor( new MonitorHandle( narrowname, widename, w, h ) ) );
	}

	return monitors;
}

} // namespace MultiLibrary