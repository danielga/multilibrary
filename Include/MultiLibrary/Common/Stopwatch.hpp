#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <MultiLibrary/Common/MutexLock.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API Stopwatch : public NonCopyable
{
public:
	Stopwatch( );
	virtual ~Stopwatch( );

	virtual void Resume( );
	virtual void Pause( );
	virtual void Reset( );

	virtual double GetElapsedTime( );

private:
	double last_update;
	double elapsed_time;
	bool running;
	Mutex update_lock;
};

} // namespace MultiLibrary