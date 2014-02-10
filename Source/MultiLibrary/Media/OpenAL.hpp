#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <string>

#if defined _WIN32 || defined __linux

	#include <AL/al.h>
	#include <AL/alc.h>

#elif defined __APPLE__

	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>

#endif

namespace MultiLibrary
{

namespace Internal
{

void alCheckError( const std::string &file, uint32_t line );

#if defined DEBUG

	#define alCheck( CALL ) ( ( CALL ), Internal::alCheckError( __FILE__, __LINE__ ) )

#else

	#define alCheck( CALL ) ( CALL )

#endif

} // namespace Internal

} // namespace MultiLibrary