#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/BaseAnchor.hpp>
#include <MultiLibrary/Common/Time.hpp>
#include <MultiLibrary/Media/SoundSource.hpp>

namespace MultiLibrary
{

class SoundBuffer;

class MULTILIBRARY_MEDIA_API Sound : public SoundSource, public BaseAnchor
{
public:
	Sound( );
	virtual ~Sound( );

	void Play( );
	void Pause( );
	void Stop( );

	void SetBuffer( const SoundBuffer &soundbuffer );
	const SoundBuffer &GetBuffer( );

	void SetLoop( bool loop );
	bool GetLoop( );

	void SetPlayingOffset( const Time &timeOffset );
	Time GetPlayingOffset( );

	void ResetAttachment( );
};

} // namespace MultiLibrary