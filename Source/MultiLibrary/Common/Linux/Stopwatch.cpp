#include <MultiLibrary/Common/Stopwatch.hpp>
#include <time.h>

namespace MultiLibrary
{

Stopwatch::Stopwatch( ) :
	last_update( 0.0 ),
	elapsed_time( 0.0 ),
	running( false )
{ }

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

	timespec time;
	clock_gettime( CLOCK_MONOTONIC, &time );
	last_update = time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
	running = true;
}

void Stopwatch::Pause( )
{
	MutexLock auto_lock( update_lock );

	if( !running )
		return;

	timespec time;
	clock_gettime( CLOCK_MONOTONIC, &time );
	elapsed_time += time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0 - last_update;
	running = false;
}

void Stopwatch::Reset( )
{
	MutexLock auto_lock( update_lock );

	elapsed_time = 0.0;
	timespec time;
	clock_gettime( CLOCK_MONOTONIC, &time );
	last_update = time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
}

double Stopwatch::GetElapsedTime( )
{
	MutexLock auto_lock( update_lock );

	if( running )
	{
		timespec time;
		clock_gettime( CLOCK_MONOTONIC, &time );
		double increment = time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
		elapsed_time += increment - last_update;
		last_update = increment;
	}

	return elapsed_time;
}

} // namespace MultiLibrary