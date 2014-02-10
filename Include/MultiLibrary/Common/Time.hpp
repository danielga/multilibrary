#pragma once

#include <MultiLibrary/Common/Export.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API Time
{
public:
	Time( );

	float Seconds( ) const;
	int32_t Milliseconds( ) const;
	int64_t Microseconds( ) const;

	static const Time Zero;

private:
	explicit Time( int64_t micros );

	friend MULTILIBRARY_COMMON_API Time Seconds( float secs );
	friend MULTILIBRARY_COMMON_API Time Milliseconds( int32_t ms );
	friend MULTILIBRARY_COMMON_API Time Microseconds( int64_t micros );

	int64_t microseconds;
};

MULTILIBRARY_COMMON_API Time Seconds( float );

MULTILIBRARY_COMMON_API Time Milliseconds( int32_t );

MULTILIBRARY_COMMON_API Time Microseconds( int64_t );

} // namespace MultiLibrary