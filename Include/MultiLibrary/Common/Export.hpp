#pragma once

#include <MultiLibrary/Config.hpp>

#if defined MULTILIBRARY_COMMON_EXPORT

	#define MULTILIBRARY_COMMON_API MULTILIBRARY_API_EXPORT

#else

	#define MULTILIBRARY_COMMON_API MULTILIBRARY_API_IMPORT

#endif