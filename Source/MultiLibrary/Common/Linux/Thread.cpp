#include <MultiLibrary/Common/Thread.hpp>
#include <pthread.h>

namespace MultiLibrary
{

Thread::~Thread( )
{
	Join( );

	if( thread_callback != NULL )
		delete thread_callback;
}

bool Thread::Launch( )
{
	Join( );
	pthread_t *thread = new pthread_t;
	if( pthread_create( thread, NULL, &Thread::Run, this ) != 0 )
		delete thread;
	else
		thread_pointer = thread;
	return thread_pointer != NULL;
}

void Thread::Detach( )
{
	if( thread_pointer != NULL )
	{
		pthread_t *thread = static_cast<pthread_t *>( thread_pointer );
		pthread_detach( *thread );
		delete thread;
		thread_pointer = NULL;
	}
}

void Thread::Terminate( )
{
	if( thread_pointer != NULL )
	{
		pthread_t *thread = static_cast<pthread_t *>( thread_pointer );
		pthread_cancel( *thread );
		delete thread;
		thread_pointer = NULL;
	}
}

void Thread::Join( )
{
	if( thread_pointer != NULL )
	{
		pthread_t *thread = static_cast<pthread_t *>( thread_pointer );
		if( pthread_equal( pthread_self( ), *thread ) == 0 )
			return;

		pthread_join( *thread, NULL );
		delete thread;
		thread_pointer = NULL;
	}
}

void Thread::Sleep( const Time &duration )
{
	int32_t microseconds = duration.Microseconds( );
	time_t secs = microseconds / 1000000;
	timespec time = { secs, ( microseconds - secs * 1000000 ) * 1000 };
	nanosleep( &time, NULL );
}

void *Thread::Run( void *userdata )
{
	pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL );
	static_cast<Thread *>( userdata )->thread_callback->Run( );
	return 0;
}

} // namespace MultiLibrary