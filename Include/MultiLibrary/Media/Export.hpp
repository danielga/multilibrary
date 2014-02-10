#pragma once

#include <MultiLibrary/Config.hpp>

#if defined MULTILIBRARY_MEDIA_EXPORT

	#define MULTILIBRARY_MEDIA_API MULTILIBRARY_API_EXPORT

#else

	#define MULTILIBRARY_MEDIA_API MULTILIBRARY_API_IMPORT

#endif