#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

AudioDevice::AudioDevice( ) :
	openal_device( NULL ),
	openal_context( NULL )
{
	OpenDevice( );
}

AudioDevice::AudioDevice( const std::string &devname ) :
	openal_device( NULL ),
	openal_context( NULL )
{
	OpenDevice( devname );
}

AudioDevice::~AudioDevice( )
{
	CloseDevice( );
}

void AudioDevice::OpenDevice( )
{
	CloseDevice( );

	if( ( openal_device = alcOpenDevice( NULL ) ) == NULL )
		return;

	if( ( openal_context = alcCreateContext( openal_device, NULL ) ) == NULL )
		return;

	MakeCurrent( );
}

void AudioDevice::OpenDevice( const std::string &devname )
{
	CloseDevice( );

	if( ( openal_device = alcOpenDevice( devname.c_str( ) ) ) == NULL )
		return;

	if( ( openal_context = alcCreateContext( openal_device, NULL ) ) == NULL )
		return;

	MakeCurrent( );
}

void AudioDevice::CloseDevice( )
{
	if( openal_context != NULL )
	{
		if( alcGetCurrentContext( ) == openal_context )
			alcMakeContextCurrent( NULL );

		alcDestroyContext( openal_context );
		openal_context = NULL;
	}

	if( openal_device != NULL )
	{
		alcCloseDevice( openal_device );
		openal_device = NULL;
	}
}

void AudioDevice::MakeCurrent( )
{
	if( openal_context != NULL )
		alCheck( alcMakeContextCurrent( openal_context ) );
}

std::vector<std::string> AudioDevice::GetAudioDevices( )
{
	std::vector<std::string> devicesList;
	const char *devicesNames = alcGetString( NULL, ALC_DEVICE_SPECIFIER );
	if( devicesNames == NULL )
		return devicesList;

	while( *devicesNames != '\0' )
	{
		std::string deviceName = devicesNames;
		devicesList.push_back( deviceName );
		devicesNames += deviceName.length( ) + 1;
	}

	return devicesList;
}

int AudioDevice::GetFormatFromChannelCount( uint32_t numChannels )
{
	switch( numChannels )
	{
		case 1:
			return AL_FORMAT_MONO16;

		case 2:
			return AL_FORMAT_STEREO16;

		case 4:
			return alGetEnumValue( "AL_FORMAT_QUAD16" );

		case 6:
			return alGetEnumValue( "AL_FORMAT_51CHN16" );

		case 7:
			return alGetEnumValue( "AL_FORMAT_61CHN16" );

		case 8:
			return alGetEnumValue( "AL_FORMAT_71CHN16" );

		default:
			return 0;
	}
}

} // namespace MultiLibrary