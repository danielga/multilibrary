#include <MultiLibrary/Media/SoundBuffer.hpp>
#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

SoundBuffer::SoundBuffer( )
{
	alCheck( alGenBuffers( 1, &buffer_id ) );
}

SoundBuffer::~SoundBuffer( )
{
	alCheck( alDeleteBuffers( 1, &buffer_id ) );
}

bool SoundBuffer::Load( const std::string &filename )
{
	MediaDecoder file;
	if( file.Open( filename ) )
		return Initialize( file );

	return false;
}

bool SoundBuffer::Load( const void *data, size_t bytes )
{
	MediaDecoder file;
	if( file.Open( data, bytes ) )
		return Initialize( file );

	return false;
}

bool SoundBuffer::Load( InputStream &stream )
{
	MediaDecoder file;
	if( file.Open( stream ) )
		return Initialize( file );

	return false;
}

bool SoundBuffer::Load( const int16_t *samples, size_t samples_count, uint32_t channel_count, uint32_t samplerate )
{
	if( samples && samples_count && channel_count && samplerate )
	{
		samples_buffer.assign( samples, samples + samples_count );
		return Update( channel_count, samplerate );
	}

	return false;
}

bool SoundBuffer::ConvertToMono( )
{
	uint32_t channels = GetChannelCount( );
	if( channels <= 1 || samples_buffer.empty( ) )
		return false;

	int32_t sample = 0;
	int16_t *data = &samples_buffer[0];
	size_t max = samples_buffer.size( ) / channels;
	for( size_t k = 0; k < max; ++k )
	{
		sample = 0;
		for( uint32_t i = 0; i < channels; ++i )
			sample += data[k * channels + i] / channels;
		data[k] = static_cast<int16_t>( sample );
	}

	samples_buffer.reserve( max );
	return Update( 1, GetSampleRate( ) );
}

const int16_t *SoundBuffer::GetSamples( ) const
{
	return samples_buffer.data( );
}

size_t SoundBuffer::GetSampleCount( ) const
{
	return samples_buffer.size( );
}

uint32_t SoundBuffer::GetSampleRate( ) const
{
	ALint sampleRate;
	alCheck( alGetBufferi( buffer_id, AL_FREQUENCY, &sampleRate ) );
	return sampleRate;
}

uint32_t SoundBuffer::GetChannelCount( ) const
{
	ALint channelCount;
	alCheck( alGetBufferi( buffer_id, AL_CHANNELS, &channelCount ) );
	return channelCount;
}

Time SoundBuffer::GetDuration( ) const
{
	return buffer_duration;
}

uint32_t SoundBuffer::GetBufferIndex( ) const
{
	return buffer_id;
}

bool SoundBuffer::Initialize( MediaDecoder &file )
{
	uint32_t channel_count = file.GetChannelCount( );
	uint32_t samplerate = file.GetSampleRate( );
	float duration = file.GetAudioDuration( );

	// This allocates a pretty close amount of memory to
	// the one required for all the samples in the file
	samples_buffer.reserve( static_cast<size_t>( duration * samplerate * channel_count ) );

	AudioFrame frame;
	while( file.ReadAudio( frame ) )
		samples_buffer.insert( samples_buffer.end( ), frame.data.begin( ), frame.data.end( ) );

	// We trim the vector capacity to only the required size since
	// we can be using tens of MB on a single audio file
	std::vector<int16_t>( samples_buffer ).swap( samples_buffer );

	return Update( channel_count, samplerate );
}

bool SoundBuffer::Update( uint32_t channels, uint32_t samplerate )
{
	if( !channels || !samplerate || samples_buffer.empty( ) )
		return false;

	ALenum format = AudioDevice::GetFormatFromChannelCount( channels );
	if( format == 0 )
		return false;

	ALsizei bytes_count = static_cast<ALsizei>( samples_buffer.size( ) * sizeof( int16_t ) );
	alCheck( alBufferData( buffer_id, format, &samples_buffer[0], bytes_count, samplerate ) );

	buffer_duration = Seconds( static_cast<float>( samples_buffer.size( ) ) / samplerate / channels );

	samples_buffer.clear( );
	std::vector<int16_t>( ).swap( samples_buffer );

	return true;
}

} // namespace MultiLibrary