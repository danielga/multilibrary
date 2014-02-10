#include <MultiLibrary/Window/Context.hpp>
#include <MultiLibrary/Window/Window.hpp>

namespace MultiLibrary
{

Context::Context( const Window &window ) :
	window_internal( window )
{
	window_internal.SetActive( true );
}

Context::~Context( )
{
	window_internal.SetActive( false );
}

} // namespace MultiLibrary