#include <MultiLibrary/Common/Mutex.hpp>
#include <pthread.h>

namespace MultiLibrary
{

Mutex::Mutex( )
{
	internal_mutex = new pthread_mutex_t;

	pthread_mutexattr_t attributes;
	pthread_mutexattr_init( &attributes );
	pthread_mutexattr_settype( &attributes, PTHREAD_MUTEX_RECURSIVE );

	pthread_mutex_init( static_cast<pthread_mutex_t *>( internal_mutex ), &attributes );
}

Mutex::~Mutex( )
{
	pthread_mutex_destroy( static_cast<pthread_mutex_t *>( internal_mutex ) );
	delete static_cast<pthread_mutex_t *>( internal_mutex );
}

void Mutex::Enter( )
{
	pthread_mutex_lock( static_cast<pthread_mutex_t *>( internal_mutex ) );
}

void Mutex::Leave( )
{
	pthread_mutex_unlock( static_cast<pthread_mutex_t *>( internal_mutex ) );
}

bool Mutex::TryEntering( )
{
	return pthread_mutex_trylock( static_cast<pthread_mutex_t *>( internal_mutex ) ) == 0;
}

} // namespace MultiLibrary