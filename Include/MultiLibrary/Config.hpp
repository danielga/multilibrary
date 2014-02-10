#pragma once

#include <stddef.h>
#include <stdint.h>

#if defined MULTILIBRARY_STATIC || ( defined __GNUC__ && __GNUC__ < 4 )

	#define MULTILIBRARY_API_EXPORT
	#define MULTILIBRARY_API_IMPORT

#else

	#if defined _WIN32

		#define MULTILIBRARY_API_EXPORT __declspec( dllexport )
		#define MULTILIBRARY_API_IMPORT __declspec( dllimport )

		#if defined _MSC_VER

			#pragma warning( disable : 4251 )

		#endif

	#elif defined __GNUC__

		#define MULTILIBRARY_API_EXPORT __attribute__( ( __visibility__( "default" ) ) )
		#define MULTILIBRARY_API_IMPORT __attribute__( ( __visibility__( "default" ) ) )

	#else

		#error This compiler is not supported by MultiLibrary.

	#endif

#endif

namespace MultiLibrary { }
namespace ML = MultiLibrary;