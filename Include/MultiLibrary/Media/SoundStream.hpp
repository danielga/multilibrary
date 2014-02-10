#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/Time.hpp>
#include <MultiLibrary/Common/Thread.hpp>
#include <MultiLibrary/Media/SoundSource.hpp>
#include <vector>

namespace MultiLibrary
{

class MULTILIBRARY_MEDIA_API SoundStream : public SoundSource
{
public:
	SoundStream( );
	virtual ~SoundStream( );

	void Play( );
	void Pause( );
	void Stop( );

	uint32_t GetChannelCount( ) const;
	uint32_t GetSampleRate( ) const;
	Time GetDuration( ) const;

	SoundStatus GetStatus( ) const;

	void SetPlayingOffset( const Time &time_offset );
	Time GetPlayingOffset( ) const;

	void SetLoop( bool looping );
	bool GetLoop( );

protected:
	void Initialize( uint32_t channels, uint32_t sampleRate, const Time &duration );

	virtual bool GetData( std::vector<int16_t> &buffer, size_t min ) = 0;
	virtual bool Seek( const Time &time_offset ) = 0;

private:
	void *DataStreamer( );
	bool FillAndPushBuffer( uint32_t buffer_num );

	static const uint8_t MAX_AUDIO_BUFFERS = 3;

	Thread stream_thread;
	volatile bool is_streaming;
	volatile bool should_loop;
	volatile uint64_t samples_processed;

	uint32_t channel_count;
	uint32_t sample_rate;
	Time audio_duration;
	int32_t audio_format;

	uint32_t audio_buffers[MAX_AUDIO_BUFFERS];
	bool end_buffer[3];

	std::vector<int16_t> temp_buffer;
	size_t minimum_bufsize;
};

} // namespace MultiLibrary