#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/Stream.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API InputStream : public virtual Stream
{
public:
	virtual size_t Read( void *data, size_t size ) = 0;
};

} // namespace MultiLibrary