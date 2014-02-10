#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/BaseAnchor.hpp>
#include <string>
#include <vector>

struct AVFormatContext;
struct AVPacket;
struct SwrContext;
struct SwsContext;

namespace MultiLibrary
{

struct AudioFrame
{
	std::vector<int16_t> data; // Packed 16 bit samples
	uint32_t samplerate;
	uint32_t channels;
};

struct VideoFrame
{
	std::vector<uint8_t> data; // Packed RGBA
	uint32_t width;
	uint32_t height;
};

class MULTILIBRARY_MEDIA_API MediaDecoder : public BaseAnchor
{
public:
	enum StreamType
	{
		STREAMTYPE_VIDEO,
		STREAMTYPE_AUDIO,

		STREAMTYPE_COUNT
	};

	MediaDecoder( );
	~MediaDecoder( );

	bool Open( const std::string &filename );
	bool Open( const void *buffer, size_t size );
	bool Open( class InputStream &in_stream );
	bool IsOpen( ) const;

	bool Close( );

	void ClearQueue( );

	const char *GetMetadata( const char *key );

	uint32_t GetChannelCount( ) const;
	uint32_t GetSampleRate( ) const;
	float GetAudioDuration( ) const;

	uint32_t GetWidth( ) const;
	uint32_t GetHeight( ) const;
	float GetFrameRate( ) const;
	float GetVideoDuration( ) const;

	bool ReadAudio( AudioFrame &audio_frame );
	bool ReadVideo( VideoFrame &video_frame );

	bool Seek( int64_t offset );
	int64_t GetPosition( );

	void IgnoreStream( StreamType stream_type, bool ignore );
	bool IsIgnoringStream( StreamType stream_type );

private:
	bool InternalOpen( const std::string &filename = "dummy" );
	bool InternalRead( StreamType type, AVPacket *packet );

	static int InternalMemoryRead( void *opaque, uint8_t *buf, int buf_size );
	static int64_t InternalMemorySeek( void *opaque, int64_t offset, int whence );

	enum DecodeMode
	{
		DECODEMODE_NONE = -1,
		DECODEMODE_FILE,
		DECODEMODE_MEMORY,
		DECODEMODE_STREAM
	};

	AVFormatContext *format_context;
	std::vector<AVPacket *> queued_packets;

	DecodeMode decoding_mode;
	const uint8_t *memory_buffer;
	int64_t memory_size;
	int64_t memory_pos;
	uint8_t *temp_memory;

	uint32_t audio_channels;
	uint32_t audio_samplerate;
	float audio_duration;

	uint32_t video_width;
	uint32_t video_height;
	float video_framerate;
	float video_duration;

	bool ignored_streams[STREAMTYPE_COUNT];

	SwrContext *swr_context;
	SwsContext *sws_context;
};

} // namespace MultiLibrary