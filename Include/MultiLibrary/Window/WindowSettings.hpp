#pragma once

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Window/Monitor.hpp>

namespace MultiLibrary
{

struct WindowSettings
{
public:
	WindowSettings( bool visible = true, bool resizable = true, bool decorated = true, const Monitor &monitor = Monitor( ) ) :
		visible( visible ),
		resizable( resizable ),
		decorated( decorated ),
		monitor( monitor )
	{ }

	bool visible;
	bool resizable;
	bool decorated;
	Monitor monitor;
};

} // namespace MultiLibrary