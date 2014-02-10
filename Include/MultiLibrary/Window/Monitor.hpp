#pragma once

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Window/VideoMode.hpp>
#include <MultiLibrary/Common/Vector2.hpp>
#include <string>
#include <vector>

struct GLFWmonitor;

namespace MultiLibrary
{

typedef GLFWmonitor *MonitorHandle;

typedef void ( *MonitorCallback ) ( const class Monitor &monitor, bool connected );

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

	bool IsValid( ) const;

	std::string GetName( ) const;

	Vector2i GetPhysicalSize( ) const;

	Vector2i GetPosition( ) const;

	VideoMode GetDesktopMode( ) const;

	std::vector<GammaColors> GetGammaRamp( ) const;
	void SetGamma( float gamma );
	void SetGammaRamp( const std::vector<GammaColors> &gamma_ramp );

	std::vector<VideoMode> GetFullscreenModes( ) const;

	void AddMonitorCallback( MonitorCallback func );
	void RemoveMonitorCallback( MonitorCallback func );

	MonitorHandle GetHandle( ) const;

	static std::vector<Monitor> GetMonitors( );

private:
	friend class Window;

	Monitor( MonitorHandle monitor );

	static void MonitorChangeCallback( MonitorHandle mon, int connected );

	MonitorHandle monitor_internal;
	static std::vector<MonitorCallback> monitor_callbacks;
};

} // namespace MultiLibrary