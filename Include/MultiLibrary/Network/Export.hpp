#pragma once

#include <MultiLibrary/Config.hpp>

#if defined MULTILIBRARY_NETWORK_EXPORT

	#define MULTILIBRARY_NETWORK_API MULTILIBRARY_API_EXPORT

#else

	#define MULTILIBRARY_NETWORK_API MULTILIBRARY_API_IMPORT

#endif