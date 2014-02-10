#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/Vector3.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_MEDIA_API Listener
{
public:
	static void SetMasterGain( float gain );
	static float GetMasterGain( );

	static void SetPosition( const Vector3f &pos );
	static Vector3f GetPosition( );

	static void SetVelocity( const Vector3f &vel );
	static Vector3f GetVelocity( );

	static void SetDirection( const Vector3f &dir );
	static Vector3f GetDirection( );
};

} // namespace MultiLibrary