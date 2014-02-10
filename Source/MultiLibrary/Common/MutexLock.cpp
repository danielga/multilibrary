#include <MultiLibrary/Common/MutexLock.hpp>

namespace MultiLibrary
{

MutexLock::MutexLock( Mutex &m ) :
	mutex( m )
{
	mutex.Enter( );
}

MutexLock::~MutexLock( )
{
	mutex.Leave( );
}

} // namespace MultiLibrary