#include <MultiLibrary/Media/Sound.hpp>
#include <MultiLibrary/Media/SoundBuffer.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

Sound::Sound( ) { }

Sound::~Sound( )
{
	Stop( );
}

void Sound::Play( )
{
	alCheck( alSourcePlay( audio_source ) );
}

void Sound::Pause( )
{
	alCheck( alSourcePause( audio_source ) );
}

void Sound::Stop( )
{
	alCheck( alSourceStop( audio_source ) );
}

void Sound::SetBuffer( const SoundBuffer &soundbuffer )
{
	Stop( );
	reinterpret_cast<SoundBuffer *>( GetAttachment( ) )->Detach( this );

	soundbuffer.Attach( this );
	SetAttachment( const_cast<SoundBuffer *>( &soundbuffer ) );
	alCheck( alSourcei( audio_source, AL_BUFFER, soundbuffer.GetBufferIndex( ) ) );
}

const SoundBuffer &Sound::GetBuffer( )
{
	return *reinterpret_cast<const SoundBuffer *>( GetAttachment( ) );
}

void Sound::SetLoop( bool loop )
{
	alCheck( alSourcei( audio_source, AL_LOOPING, loop ) );
}

bool Sound::GetLoop( )
{
	ALint loop;
	alCheck( alGetSourcei( audio_source, AL_LOOPING, &loop ) );
	return loop != 0;
}

void Sound::SetPlayingOffset( const Time &timeOffset )
{
	alCheck( alSourcef( audio_source, AL_SEC_OFFSET, timeOffset.Seconds( ) ) );
}

Time Sound::GetPlayingOffset( )
{
	ALfloat secs = 0.0f;
	alCheck( alGetSourcef( audio_source, AL_SEC_OFFSET, &secs ) );
	return Seconds( secs );
}

void Sound::ResetAttachment( )
{
	Stop( );
	alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
	BaseAnchor::ResetAttachment( );
}

} // namespace MultiLibrary
