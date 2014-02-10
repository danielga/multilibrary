#pragma once

#include <MultiLibrary/Visual/Export.hpp>
#include <string>
#include <GL/glew.h>

namespace MultiLibrary
{

namespace Internal
{

void glCheckError( const std::string &file, uint32_t line );

#if defined DEBUG

	#define glCheck( CALL ) ( ( CALL ), Internal::glCheckError( __FILE__, __LINE__ ) )

#else

	#define glCheck( CALL ) ( CALL )

#endif

} // namespace Internal

} // namespace MultiLibrary