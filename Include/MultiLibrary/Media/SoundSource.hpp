#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/Vector3.hpp>

namespace MultiLibrary
{

const unsigned char MAX_BUFFERS = 2;

enum SoundStatus
{
	Stopped,
	Paused,
	Playing
};

class MULTILIBRARY_MEDIA_API SoundSource
{
public:
	SoundSource( );
	virtual ~SoundSource( );

	void SetPitch( float pitch );
	float GetPitch( ) const;

	void SetGain( float gain );
	float GetGain( ) const;

	void SetPosition( const Vector3f &pos );
	Vector3f GetPosition( );

	void SetRelativeToListener( bool relative );
	bool IsRelativeToListener( ) const;

	void SetMinDistance( float distance );
	float GetMinDistance( ) const;

	void SetAttenuation( float attenuation );
	float GetAttenuation( ) const;

	SoundStatus GetStatus( ) const;

protected:
	unsigned int audio_source;
};

} // namespace MultiLibrary