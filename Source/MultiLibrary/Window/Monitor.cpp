#include <MultiLibrary/Window/Monitor.hpp>
#include <MultiLibrary/Window/GLFW.hpp>

namespace MultiLibrary
{

Monitor::Monitor( bool use_primary_monitor ) :
	monitor_internal( NULL )
{
	Internal::InitializeGLFW( );

	if( use_primary_monitor )
		monitor_internal = glfwGetPrimaryMonitor( );
}

bool Monitor::IsValid( ) const
{
	return monitor_internal != NULL;
}

std::string Monitor::GetName( ) const
{
	if( !IsValid( ) )
		return std::string( );

	return glfwGetMonitorName( monitor_internal );
}

Vector2i Monitor::GetPhysicalSize( ) const
{
	Vector2i size;
	if( !IsValid( ) )
		return size;

	glfwGetMonitorPhysicalSize( monitor_internal, &size.x, &size.y );
	return size;
}

Vector2i Monitor::GetPosition( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	glfwGetMonitorPos( monitor_internal, &pos.x, &pos.y );
	return pos;
}

VideoMode Monitor::GetDesktopMode( ) const
{
	if( !IsValid( ) )
		return VideoMode( );

	const GLFWvidmode *vm = glfwGetVideoMode( monitor_internal );
	if( vm == NULL )
		return VideoMode( );

	return VideoMode( vm->width, vm->height, vm->redBits, vm->greenBits, vm->blueBits, vm->refreshRate );
}

std::vector<GammaColors> Monitor::GetGammaRamp( ) const
{
	std::vector<GammaColors> ramp;
	if( !IsValid( ) )
		return ramp;

	const GLFWgammaramp *gr = glfwGetGammaRamp( monitor_internal );
	for( uint32_t k = 0; k < gr->size; ++k )
		ramp.push_back( GammaColors( gr->red[k], gr->green[k], gr->blue[k] ) );

	return ramp;
}

void Monitor::SetGamma( float gamma )
{
	if( !IsValid( ) )
		return;

	glfwSetGamma( monitor_internal, gamma );
}

void Monitor::SetGammaRamp( const std::vector<GammaColors> &gamma_ramp )
{
	if( !IsValid( ) )
		return;

	size_t size = gamma_ramp.size( );
	std::vector<uint16_t> reds;
	reds.reserve( size );
	std::vector<uint16_t> blues;
	blues.reserve( size );
	std::vector<uint16_t> greens;
	greens.reserve( size );

	std::vector<GammaColors>::const_iterator end = gamma_ramp.end( );
	for( std::vector<GammaColors>::const_iterator it = gamma_ramp.begin( ); it != end; ++it )
	{
		const GammaColors &gamma = *it;
		reds.push_back( gamma.red );
		greens.push_back( gamma.green );
		blues.push_back( gamma.blue );
	}

	GLFWgammaramp ramp;
	ramp.red = &reds[0];
	ramp.green = &greens[0];
	ramp.blue = &blues[0];
	ramp.size = size;
	glfwSetGammaRamp( monitor_internal, &ramp );
}

std::vector<VideoMode> Monitor::GetFullscreenModes( ) const
{
	std::vector<VideoMode> video_modes;
	if( !IsValid( ) )
		return video_modes;

	int32_t count = 0;
	for( const GLFWvidmode *v = glfwGetVideoModes( monitor_internal, &count ), *e = v + count; v != e; ++v )
	{
		const GLFWvidmode &vm = *v;
		video_modes.push_back( VideoMode( vm.width, vm.height, vm.redBits, vm.greenBits, vm.blueBits, vm.refreshRate ) );
	}

	return video_modes;
}

void Monitor::AddMonitorCallback( MonitorCallback func )
{
	std::vector<MonitorCallback>::iterator end = monitor_callbacks.end( );
	for( std::vector<MonitorCallback>::iterator it = monitor_callbacks.begin( ); it != end; ++it )
		if( *it == func )
			return;

	monitor_callbacks.push_back( func );
}

void Monitor::RemoveMonitorCallback( MonitorCallback func )
{
	std::vector<MonitorCallback>::iterator end = monitor_callbacks.end( );
	for( std::vector<MonitorCallback>::iterator it = monitor_callbacks.begin( ); it != end; ++it )
		if( *it == func )
		{
			monitor_callbacks.erase( it );
			return;
		}
}

MonitorHandle Monitor::GetHandle( ) const
{
	return monitor_internal;
}

std::vector<Monitor> Monitor::GetMonitors( )
{
	std::vector<Monitor> monitors;
	int32_t count = 0;
	for( MonitorHandle *m = glfwGetMonitors( &count ), *e = m + count; m != e; ++m )
		monitors.push_back( Monitor( *m ) );

	return monitors;
}

Monitor::Monitor( MonitorHandle monitor ) :
	monitor_internal( monitor )
{
	// Is this really required? Whenever we receive a glfw monitor
	// handle that library is probably already initialized.
	Internal::InitializeGLFW( );
}

void Monitor::MonitorChangeCallback( MonitorHandle mon, int connected )
{
	Monitor monitor( mon );
	bool isconnected = connected == GLFW_CONNECTED;

	std::vector<MonitorCallback>::iterator end = monitor_callbacks.end( );
	for( std::vector<MonitorCallback>::iterator it = monitor_callbacks.begin( ); it != end; ++it )
		( *it )( monitor, isconnected );
}

std::vector<MonitorCallback> Monitor::monitor_callbacks;

} // namespace MultiLibrary