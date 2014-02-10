#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API Mutex : public NonCopyable
{
public:
	Mutex( );
	~Mutex( );

	void Enter( );
	void Leave( );
	bool TryEntering( );

private:
	void *internal_mutex;
};

} // namespace MultiLibrary