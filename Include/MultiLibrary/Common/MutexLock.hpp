#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/Mutex.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API MutexLock : public NonCopyable
{
public:
	MutexLock( Mutex &m );
	~MutexLock( );

private:
	Mutex &mutex;
};

} // namespace MultiLibrary