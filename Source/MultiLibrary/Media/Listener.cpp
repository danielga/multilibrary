#include <MultiLibrary/Media/Listener.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

void Listener::SetMasterGain( float gain )
{
	alCheck( alListenerf( AL_GAIN, gain ) );
}

float Listener::GetMasterGain( )
{
	float gain = 0.0f;
	alCheck( alGetListenerf( AL_GAIN, &gain ) );
	return gain;
}

void Listener::SetPosition( const Vector3f &pos )
{
	alCheck( alListenerfv( AL_POSITION, reinterpret_cast<const ALfloat *>( &pos ) ) );
}

Vector3f Listener::GetPosition( )
{
	Vector3f pos;
	alCheck( alGetListenerfv( AL_POSITION, reinterpret_cast<ALfloat *>( &pos ) ) );
	return pos;
}

void Listener::SetVelocity( const Vector3f &vel )
{
	alCheck( alListenerfv( AL_VELOCITY, reinterpret_cast<const ALfloat *>( &vel ) ) );
}

Vector3f Listener::GetVelocity( )
{
	Vector3f vel;
	alCheck( alGetListenerfv( AL_VELOCITY, reinterpret_cast<ALfloat *>( &vel ) ) );
	return vel;
}

void Listener::SetDirection( const Vector3f &dir )
{
	float orientation[] = { dir.x, dir.y, dir.z, 0.0f, 1.0f, 0.0f };
	alCheck( alListenerfv( AL_ORIENTATION, orientation ) );
}

Vector3f Listener::GetDirection( )
{
	float orientation[6];
	alCheck( alGetListenerfv( AL_ORIENTATION, orientation ) );
	return Vector3f( orientation[0], orientation[1], orientation[2] );
}

} // namespace MultiLibrary