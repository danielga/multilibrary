#include <MultiLibrary/Media/SoundStream.hpp>
#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/OpenAL.hpp>

namespace MultiLibrary
{

SoundStream::SoundStream( ) :
	stream_thread( &SoundStream::DataStreamer, this ),
	is_streaming( false ),
	should_loop( false ),
	samples_processed( 0 ),
	channel_count( 0 ),
	sample_rate( 0 ),
	audio_format( 0 ),
	minimum_bufsize( 0 )
{ }

SoundStream::~SoundStream( )
{
	Stop( );
}

void SoundStream::Play( )
{
	if( is_streaming )
	{
		alCheck( alSourcePlay( audio_source ) );
		return;
	}

	Seek( Time::Zero );

	samples_processed = 0;
	is_streaming = true;
	stream_thread.Launch( );
}

void SoundStream::Pause( )
{
	alCheck( alSourcePause( audio_source ) );
}

void SoundStream::Stop( )
{
	is_streaming = false;
	stream_thread.Join( );
}

unsigned int SoundStream::GetChannelCount( ) const
{
	return channel_count;
}

unsigned int SoundStream::GetSampleRate( ) const
{
	return sample_rate;
}

Time SoundStream::GetDuration( ) const
{
	return audio_duration;
}

SoundStatus SoundStream::GetStatus( ) const
{
	SoundStatus status = SoundSource::GetStatus( );

	if( status == Stopped && is_streaming )
		status = Playing;

	return status;
}

void SoundStream::SetPlayingOffset( const Time &time_offset )
{
	Stop( );

	Seek( time_offset );

	samples_processed = static_cast<uint64_t>( time_offset.Seconds( ) * sample_rate * channel_count );
	is_streaming = true;
	stream_thread.Launch( );
}

Time SoundStream::GetPlayingOffset( ) const
{
	ALfloat secs = 0.0f;
	alCheck( alGetSourcef( audio_source, AL_SEC_OFFSET, &secs ) );
	return Seconds( secs + samples_processed / static_cast<float>( sample_rate ) / channel_count );
}

void SoundStream::SetLoop( bool looping )
{
	should_loop = looping;
}

bool SoundStream::GetLoop( )
{
	return should_loop;
}

void SoundStream::Initialize( uint32_t channels, uint32_t sampleRate, const Time &duration )
{
	channel_count = channels;
	sample_rate = sampleRate;
	audio_duration = duration;
	audio_format = AudioDevice::GetFormatFromChannelCount( channels );

	minimum_bufsize = sample_rate * channel_count * sizeof( int16_t ) / 10;

	temp_buffer.resize( minimum_bufsize );
	std::vector<int16_t>( temp_buffer ).swap( temp_buffer );
	temp_buffer.resize( 0 );
}

void *SoundStream::DataStreamer( )
{
	alCheck( alGenBuffers( MAX_AUDIO_BUFFERS, audio_buffers ) );

	bool requestStop = false;
	for( uint8_t i = 0; i < MAX_AUDIO_BUFFERS; ++i )
	{
		end_buffer[i] = false;
		if( !requestStop && FillAndPushBuffer( i ) )
			requestStop = true;
	}

	alCheck( alSourcePlay( audio_source ) );

	while( is_streaming )
	{
		if( SoundSource::GetStatus( ) == Stopped )
		{
			if( !requestStop )
				alCheck( alSourcePlay( audio_source ) );
			else
				is_streaming = false;
		}

		ALint nbProcessed = 0;
		alCheck( alGetSourcei( audio_source, AL_BUFFERS_PROCESSED, &nbProcessed ) );

		while( nbProcessed-- )
		{
			ALuint buffer;
			alCheck( alSourceUnqueueBuffers( audio_source, 1, &buffer ) );

			uint32_t bufferNum = 0;
			for( uint8_t i = 0; i < MAX_AUDIO_BUFFERS; ++i )
				if( audio_buffers[i] == buffer )
				{
					bufferNum = i;
					break;
				}

			if( end_buffer[bufferNum] )
			{
				samples_processed = 0;
				end_buffer[bufferNum] = false;
			}
			else
			{
				ALint size, bits;
				alCheck( alGetBufferi( buffer, AL_SIZE, &size ) );
				alCheck( alGetBufferi( buffer, AL_BITS, &bits ) );
				samples_processed += size / ( bits / 8 );
			}

			if( !requestStop )
				if( FillAndPushBuffer( bufferNum ) )
					requestStop = true;
		}

		if( SoundSource::GetStatus( ) != Stopped )
			CurrentThread::Sleep( ML::Milliseconds( 10 ) );
	}

	alCheck( alSourceStop( audio_source ) );

	ALint nbQueued = 0;
	alCheck( alGetSourcei( audio_source, AL_BUFFERS_QUEUED, &nbQueued ) );

	ALuint buffer = 0;
	for( ALint i = 0; i < nbQueued; ++i )
		alCheck( alSourceUnqueueBuffers( audio_source, 1, &buffer ) );

	alCheck( alSourcei( audio_source, AL_BUFFER, 0 ) );
	alCheck( alDeleteBuffers( MAX_AUDIO_BUFFERS, audio_buffers ) );

	return NULL;
}

bool SoundStream::FillAndPushBuffer( uint32_t buffer_num )
{
	bool requestStop = false;

	if( !GetData( temp_buffer, minimum_bufsize ) )
	{
		end_buffer[buffer_num] = true;

		if( should_loop )
		{
			Seek( Time::Zero );

			if( temp_buffer.size( ) == 0 )
				return FillAndPushBuffer( buffer_num );
		}
		else
		{
			requestStop = true;
		}
	}

	if( temp_buffer.size( ) > 0 )
	{
		uint32_t buffer = audio_buffers[buffer_num];

		ALsizei size = static_cast<ALsizei>( temp_buffer.size( ) * sizeof( int16_t ) );
		alCheck( alBufferData( buffer, audio_format, &temp_buffer[0], size, sample_rate ) );

		alCheck( alSourceQueueBuffers( audio_source, 1, &buffer ) );
	}

	temp_buffer.resize( 0 );

	return requestStop;
}

} // namespace MultiLibrary