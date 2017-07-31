/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015-2017, Daniel Almeida
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <chrono>
#include <string>
#include <vector>
#include <memory>

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

class MULTILIBRARY_MEDIA_API MediaDecoder : public NonCopyable
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
	std::chrono::microseconds GetAudioDuration( ) const;

	uint32_t GetWidth( ) const;
	uint32_t GetHeight( ) const;
	float GetFrameRate( ) const;
	std::chrono::microseconds GetVideoDuration( ) const;

	bool ReadAudio( AudioFrame &audio_frame );
	bool ReadVideo( VideoFrame &video_frame );

	bool Seek( int64_t offset );
	int64_t GetPosition( );

	void IgnoreStream( StreamType stream_type, bool ignore );
	bool IsIgnoringStream( StreamType stream_type );

	void ResetPublisher( );

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

	typedef void ( *MemoryFreer )( void * );

	AVFormatContext *format_context;
	std::vector<AVPacket *> queued_packets;

	DecodeMode decoding_mode;
	const uint8_t *memory_buffer;
	int64_t memory_size;
	int64_t memory_pos;
	std::unique_ptr<uint8_t, MemoryFreer> temp_memory;
	InputStream *input_stream;

	uint32_t audio_channels;
	uint32_t audio_samplerate;
	std::chrono::microseconds audio_duration;

	uint32_t video_width;
	uint32_t video_height;
	float video_framerate;
	std::chrono::microseconds video_duration;

	bool ignored_streams[STREAMTYPE_COUNT];

	SwrContext *swr_context;
	SwsContext *sws_context;
};

} // namespace MultiLibrary
