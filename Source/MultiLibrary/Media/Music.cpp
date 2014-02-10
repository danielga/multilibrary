#include <MultiLibrary/Media/Music.hpp>

namespace MultiLibrary
{

Music::Music( )
{
	media_decoder.IgnoreStream( MediaDecoder::STREAMTYPE_VIDEO, true );
}

Music::~Music( )
{
	media_decoder.Close( );
}

bool Music::Open( const std::string &filename )
{
	media_decoder.Close( );

	if( media_decoder.Open( filename ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		Time duration = Seconds( media_decoder.GetAudioDuration( ) );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::Open( const void *data, size_t data_size )
{
	media_decoder.Close( );

	if( media_decoder.Open( data, static_cast<int64_t>( data_size ) ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		Time duration = Seconds( media_decoder.GetAudioDuration( ) );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::Open( InputStream &stream )
{
	media_decoder.Close( );

	if( media_decoder.Open( stream ) )
	{
		uint32_t channels = media_decoder.GetChannelCount( );
		uint32_t rate = media_decoder.GetSampleRate( );
		Time duration = Seconds( media_decoder.GetAudioDuration( ) );
		Initialize( channels, rate, duration );
		return true;
	}

	return false;
}

bool Music::GetData( std::vector<int16_t> &buffer, size_t min )
{
	AudioFrame audio_frame;
	while( buffer.size( ) < min && media_decoder.ReadAudio( audio_frame ) )
		buffer.insert( buffer.end( ), audio_frame.data.begin( ), audio_frame.data.end( ) );

	return buffer.size( ) >= min;
}

bool Music::Seek( const Time &time_offset )
{
	return media_decoder.Seek( time_offset.Microseconds( ) );
}

} // namespace MultiLibrary