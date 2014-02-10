#define WIN32_LEAN_AND_MEAN
#include <MultiLibrary/Common/Mutex.hpp>
#include <windows.h>

namespace MultiLibrary
{

Mutex::Mutex( )
{
	internal_mutex = new CRITICAL_SECTION;
	InitializeCriticalSection( static_cast<CRITICAL_SECTION *>( internal_mutex ) );
}

Mutex::~Mutex( )
{
	DeleteCriticalSection( static_cast<CRITICAL_SECTION *>( internal_mutex ) );
	delete static_cast<CRITICAL_SECTION *>( internal_mutex );
}

void Mutex::Enter( )
{
	EnterCriticalSection( static_cast<CRITICAL_SECTION *>( internal_mutex ) );
}

void Mutex::Leave( )
{
	LeaveCriticalSection( static_cast<CRITICAL_SECTION *>( internal_mutex ) );
}

bool Mutex::TryEntering( )
{
	return TryEnterCriticalSection( static_cast<CRITICAL_SECTION *>( internal_mutex ) ) != 0;
}

} // namespace MultiLibrary