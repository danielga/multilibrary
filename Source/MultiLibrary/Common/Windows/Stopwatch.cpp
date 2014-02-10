#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Common/Stopwatch.hpp>
#include <windows.h>

static int64_t update_frequency = 0;

namespace MultiLibrary
{

Stopwatch::Stopwatch( ) :
	last_update( 0.0 ),
	elapsed_time( 0.0 ),
	running( false )
{
	if( update_frequency == 0 )
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency( &freq );
		update_frequency = freq.QuadPart;
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

	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	last_update = time.QuadPart * 1000.0 / update_frequency;
	running = true;
}

void Stopwatch::Pause( )
{
	MutexLock auto_lock( update_lock );

	if( !running )
		return;

	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	elapsed_time += time.QuadPart * 1000.0 / update_frequency - last_update;
	running = false;
}

void Stopwatch::Reset( )
{
	MutexLock auto_lock( update_lock );

	elapsed_time = 0.0;
	LARGE_INTEGER time;
	QueryPerformanceCounter( &time );
	last_update = time.QuadPart * 1000.0 / update_frequency;
}

double Stopwatch::GetElapsedTime( )
{
	MutexLock auto_lock( update_lock );

	if( running )
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter( &time );
		double increment = time.QuadPart * 1000.0 / update_frequency;
		elapsed_time += increment - last_update;
		last_update = increment;
	}

	return elapsed_time;
}

} // namespace MultiLibrary