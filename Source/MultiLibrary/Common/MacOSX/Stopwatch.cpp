#include <MultiLibrary/Common/Stopwatch.hpp>
#include <mach/mach_time.h>
double update_frequency = 0.0;

namespace MultiLibrary
{

Stopwatch::Stopwatch( ) :
	last_update( 0.0 ),
	elapsed_time( 0.0 ),
	running( false )
{
	if( frequency == 0.0 )
	{
		mach_timebase_info_data_t timebaseInfo;
		mach_timebase_info( &timebaseInfo );
		update_frequency = timebaseInfo.numer / timebaseInfo.denom;
	}
}

Stopwatch::~Stopwatch( )
{
	MutexLock auto_lock( update_lock );
	running = false;
}

void Stopwatch::Resume( )
{
	MutexLock auto_lock( update_lock );

	if( running )
		return;

	last_update = mach_absolute_time( ) * update_frequency;
	running = true;
}

void Stopwatch::Pause( )
{
	MutexLock auto_lock( update_lock );

	if( !running )
		return;

	elapsed_time += mach_absolute_time( ) * update_frequency - last_update;
	running = false;
}

void Stopwatch::Reset( )
{
	MutexLock auto_lock( update_lock );

	elapsed_time = 0.0;
	last_update = mach_absolute_time( ) * update_frequency;
}

double Stopwatch::GetElapsedTime( )
{
	MutexLock auto_lock( update_lock );

	if( running )
	{
		double increment = mach_absolute_time( ) * update_frequency;
		elapsed_time += increment - last_update;
		last_update = increment;
	}

	return elapsed_time;
}

} // namespace MultiLibrary