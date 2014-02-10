#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Common/Thread.hpp>
#include <windows.h>
#include <process.h>

namespace MultiLibrary
{

struct Thread::Data
{
	Data( FunctorBase *cb, HANDLE th = NULL ) :
		pointer( th ),
		callback( cb ),
		retvalue( NULL )
	{ }

	~Data( )
	{
		if( callback != NULL )
			delete callback;
	}

	HANDLE pointer;
	FunctorBase *callback;
	volatile void *retvalue;
};

namespace Internal
{

DWORD __stdcall ThreadRun( void *userdata )
{
	Thread::Data *thread_data = static_cast<Thread::Data *>( userdata );
	thread_data->retvalue = thread_data->callback->Run( );
	return 0;
}

}

namespace CurrentThread
{

void Sleep( const Time &duration )
{
	::Sleep( duration.Milliseconds( ) );
}

Thread Get( )
{
	return Thread( new Thread::Data( NULL, GetCurrentThread( ) ) );
}

} // namespace CurrentThread

Thread::Thread( Data *data ) :
	thread_data( data )
{ }

void Thread::Init( FunctorBase *func )
{
	thread_data = new Data( func );
}

Thread::~Thread( )
{
	Join( );

	if( thread_data != NULL )
		delete thread_data;
}

bool Thread::Launch( )
{
	Join( );

	thread_data->pointer = CreateThread( NULL, 0, &Internal::ThreadRun, thread_data, 0, NULL );
	if( thread_data->pointer != NULL )
	{
		thread_data->retvalue = NULL;
		return true;
	}

	return false;
}

void Thread::Detach( )
{
	if( thread_data->pointer != NULL )
	{
		CloseHandle( thread_data->pointer );
		thread_data->pointer = NULL;
	}
}

void Thread::Terminate( )
{
	if( thread_data->pointer != NULL )
	{
		TerminateThread( thread_data->pointer, 0 );
		CloseHandle( thread_data->pointer );
		thread_data->pointer = NULL;
	}
}

void Thread::Join( )
{
	if( thread_data->pointer != NULL )
	{
		if( GetCurrentThreadId( ) == GetThreadId( thread_data->pointer ) )
			return;

		WaitForSingleObject( thread_data->pointer, INFINITE );
		CloseHandle( thread_data->pointer );
		thread_data->pointer = NULL;
	}
}

void *Thread::GetReturnValue( ) const
{
	return const_cast<void *>( thread_data->retvalue );
}

} // namespace MultiLibrary