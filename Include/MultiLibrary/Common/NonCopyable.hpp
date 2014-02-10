#pragma once

#include <MultiLibrary/Common/Export.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API NonCopyable
{
protected:
	NonCopyable( ) { }

private:
	NonCopyable( const NonCopyable & );
	NonCopyable &operator=( const NonCopyable & );
};

} // namespace MultiLibrary