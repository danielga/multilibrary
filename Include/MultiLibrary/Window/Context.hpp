#pragma once

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>

namespace MultiLibrary
{

class Window;

class MULTILIBRARY_WINDOW_API Context : public NonCopyable
{
public:
	Context( const Window &handle );
	~Context( );

private:
	const Window &window_internal;
};

} // namespace MultiLibrary