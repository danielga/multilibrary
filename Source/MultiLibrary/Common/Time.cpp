#include <MultiLibrary/Common/Time.hpp>

namespace MultiLibrary
{

const Time Time::Zero;

Time::Time( ) : microseconds( 0 ) { }

float Time::Seconds( ) const
{
	return microseconds / 1000000.0f;
}

int32_t Time::Milliseconds( ) const
{
	return static_cast<int32_t>( microseconds / 1000 );
}

int64_t Time::Microseconds( ) const
{
	return microseconds;
}

Time::Time( int64_t micros ) : microseconds( micros ) { }

MULTILIBRARY_COMMON_API Time Seconds( float secs )
{
	return Time( static_cast<int64_t>( secs * 1000000 ) );
}

MULTILIBRARY_COMMON_API Time Milliseconds( int32_t ms )
{
	return Time( static_cast<int64_t>( ms * 1000 ) );
}

MULTILIBRARY_COMMON_API Time Microseconds( int64_t micros )
{
	return Time( micros );
}

}