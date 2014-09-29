/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
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

#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <MultiLibrary/Common/InputStream.hpp>
#include <cstring>
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
	#include <libswscale/swscale.h>
	#include <libavutil/pixdesc.h>
	#include <libavutil/channel_layout.h>
}

namespace MultiLibrary
{

MediaDecoder::MediaDecoder( ) :
	format_context( nullptr ),
	decoding_mode( DECODEMODE_NONE ),
	memory_buffer( nullptr ),
	memory_size( 0 ),
	memory_pos( 0 ),
	temp_memory( nullptr, av_free ),
	input_stream( nullptr ),
	audio_channels( 0 ),
	audio_samplerate( 0 ),
	audio_duration( std::chrono::microseconds::zero( ) ),
	video_width( 0 ),
	video_height( 0 ),
	video_framerate( 0.0f ),
	video_duration( std::chrono::microseconds::zero( ) ),
	swr_context( nullptr ),
	sws_context( nullptr )
{
	for( int32_t k = 0; k < STREAMTYPE_COUNT; ++k )
		ignored_streams[k] = false;

	av_register_all( );
}

MediaDecoder::~MediaDecoder( )
{
	Close( );
}

bool MediaDecoder::Open( const std::string &filename )
{
	if( IsOpen( ) )
		return false;

	decoding_mode = DECODEMODE_FILE;
	return InternalOpen( filename );
}

bool MediaDecoder::Open( const void *buffer, size_t size )
{
	if( IsOpen( ) )
		return false;

	decoding_mode = DECODEMODE_MEMORY;
	memory_buffer = static_cast<const uint8_t *>( buffer );
	memory_size = size;
	memory_pos = 0;

	if( ( format_context = avformat_alloc_context( ) ) == nullptr )
	{
		Close( );
		return false;
	}

	temp_memory.reset( reinterpret_cast<uint8_t *>( av_malloc( 8192 ) ) );
	format_context->pb = avio_alloc_context( temp_memory.get( ), 8192, 0, this, &InternalMemoryRead, nullptr, &InternalMemorySeek );

	if( format_context->pb == nullptr )
	{
		Close( );
		return false;
	}

	return InternalOpen( );
}

bool MediaDecoder::Open( InputStream &in_stream )
{
	if( IsOpen( ) )
		return false;

	decoding_mode = DECODEMODE_STREAM;

	input_stream = &in_stream;

	if( ( format_context = avformat_alloc_context( ) ) == nullptr )
	{
		Close( );
		return false;
	}

	temp_memory.reset( reinterpret_cast<uint8_t *>( av_malloc( 8192 ) ) );
	format_context->pb = avio_alloc_context( temp_memory.get( ), 8192, 0, this, &InternalMemoryRead, nullptr, &InternalMemorySeek );

	if( format_context->pb == nullptr )
	{
		Close( );
		return false;
	}

	return InternalOpen( );
}

bool MediaDecoder::IsOpen( ) const
{
	return format_context != nullptr;
}

bool MediaDecoder::Close( )
{
	if( !IsOpen( ) )
		return false;

	ClearQueue( );

	for( uint32_t i = 0; i < format_context->nb_streams; i++ )
	{
		AVCodecContext *codec_context = format_context->streams[i]->codec;
		if( codec_context != nullptr && codec_context->codec != nullptr )
			avcodec_close( codec_context );
	}

	if( swr_context != nullptr )
		swr_free( &swr_context );

	if( sws_context != nullptr )
	{
		sws_freeContext( sws_context );
		sws_context = nullptr;
	}

	if( decoding_mode == DECODEMODE_FILE )
	{
		avformat_close_input( &format_context );
	}
	else if( decoding_mode != DECODEMODE_NONE )
	{
		av_free( format_context->pb );
		format_context->pb = nullptr;

		avformat_free_context( format_context );
		format_context = nullptr;
	}

	if( input_stream != nullptr )
		input_stream = nullptr;

	decoding_mode = DECODEMODE_NONE;
	memory_buffer = nullptr;
	memory_size = 0;
	memory_pos = 0;

	audio_channels = 0;
	audio_samplerate = 0;
	audio_duration = std::chrono::microseconds::zero( );

	video_width = 0;
	video_height = 0;
	video_framerate = 0.0f;
	video_duration = std::chrono::microseconds::zero( );

	return true;
}

void MediaDecoder::ClearQueue( )
{
	std::vector<AVPacket *>::iterator it = queued_packets.begin( );
	while( it != queued_packets.end( ) )
	{
		AVPacket *temp = *it;
		av_free_packet( temp );
		av_free( temp );

		++it;
	}

	queued_packets.clear( );
}

const char *MediaDecoder::GetMetadata( const char *key )
{
	if( !IsOpen( ) )
		return 0;

	AVDictionaryEntry *entry = av_dict_get( format_context->metadata, key, nullptr, 0 );
	return entry != nullptr ? entry->value : nullptr;
}

uint32_t MediaDecoder::GetChannelCount( ) const
{
	return audio_channels;
}

uint32_t MediaDecoder::GetSampleRate( ) const
{
	return audio_samplerate;
}

std::chrono::microseconds MediaDecoder::GetAudioDuration( ) const
{
	return audio_duration;
}

uint32_t MediaDecoder::GetWidth( ) const
{
	return video_width;
}

uint32_t MediaDecoder::GetHeight( ) const
{
	return video_height;
}

std::chrono::microseconds MediaDecoder::GetVideoDuration( ) const
{
	return video_duration;
}

float MediaDecoder::GetFrameRate( ) const
{
	return video_framerate;
}

static void CopyAudioFrame( SwrContext *swr, const AVFrame *frame, AudioFrame &audio_frame )
{
	int channels = frame->channels;
	int nbsamples = frame->nb_samples;

	audio_frame.data.resize( nbsamples * channels );

	uint8_t *temp_array[] = { reinterpret_cast<uint8_t *>( &audio_frame.data[0] ) };

	swr_convert( swr, temp_array, nbsamples, const_cast<const uint8_t **>( frame->extended_data ), nbsamples );

	audio_frame.samplerate = frame->sample_rate;
	audio_frame.channels = channels;
}

bool MediaDecoder::ReadAudio( AudioFrame &audio_frame )
{
	if( !IsOpen( ) )
		return false;

	AVFrame *frame = av_frame_alloc( );

	int len = 0;
	int frame_finished = 0;
	int done = 0;
	AVPacket packet;
	while( done == 0 && InternalRead( STREAMTYPE_AUDIO, &packet ) )
	{
		uint8_t *data = packet.data;
		int size = packet.size;
		while( packet.size > 0 )
		{
			if( packet.pos == -1 )
			{
				// packet is useless and so skip it
				packet.size = 0;
			}
			else
			{
				len += avcodec_decode_audio4( format_context->streams[packet.stream_index]->codec, frame, &frame_finished, &packet );

				if( len <= 0 )
					break;
				else if( frame_finished != 0 )
					done += len;

				packet.data += len;
				packet.size -= len;
			}
		}

		packet.data = data;
		packet.size = size;
		av_free_packet( &packet );
	}

	if( done > 0 )
	{
		CopyAudioFrame( swr_context, frame, audio_frame );
		av_frame_free( &frame );
		return true;
	}

	av_frame_free( &frame );
	return false;
}

static void CopyVideoFrame( SwsContext *sws, const AVFrame *frame, VideoFrame &video_frame )
{
	AVPixelFormat format = static_cast<AVPixelFormat>( frame->format );
	int planes = av_pix_fmt_count_planes( format );
	int width = frame->width;
	int height = frame->height;

	video_frame.data.resize( width * height * planes );

	uint8_t *data[] = { &video_frame.data[0] };

	int strides[] = { planes * width, 0, 0, 0 };
	sws_scale( sws, frame->extended_data, frame->linesize, 0, height, data, strides );

	video_frame.width = width;
	video_frame.height = height;
}

bool MediaDecoder::ReadVideo( VideoFrame &video_frame )
{
	if( !IsOpen( ) )
		return false;

	AVFrame *frame = av_frame_alloc( );

	int len = 0;
	int frame_finished = 0;
	int done = 0;
	AVPacket packet;
	while( done == 0 && InternalRead( STREAMTYPE_VIDEO, &packet ) )
	{
		uint8_t *data = packet.data;
		int size = packet.size;
		while( packet.size > 0 )
		{
			if( packet.pos == -1 )
			{
				// packet is useless and so skip it
				packet.size = 0;
			}
			else
			{
				len += avcodec_decode_video2( format_context->streams[packet.stream_index]->codec, frame, &frame_finished, &packet );

				if( len <= 0 )
					break;
				else if( frame_finished != 0 )
					done += len;

				packet.data += len;
				packet.size -= len;
			}
		}

		packet.data = data;
		packet.size = size;
		av_free_packet( &packet );
	}

	if( done > 0 )
	{
		CopyVideoFrame( sws_context, frame, video_frame );
		av_frame_free( &frame );
		return true;
	}

	av_frame_free( &frame );
	return false;
}

bool MediaDecoder::Seek( int64_t offset )
{
	if( !IsOpen( ) )
		return false;

	return av_seek_frame( format_context, -1, offset, 0 ) >= 0;
}

long long MediaDecoder::GetPosition( )
{
	if( !IsOpen( ) )
		return 0;

	return 0;
}

void MediaDecoder::IgnoreStream( StreamType stream_type, bool ignore )
{
	if( stream_type >= 0 && stream_type < STREAMTYPE_COUNT )
		ignored_streams[stream_type] = ignore;
}

bool MediaDecoder::IsIgnoringStream( StreamType stream_type )
{
	if( stream_type >= 0 && stream_type < STREAMTYPE_COUNT )
		return ignored_streams[stream_type];

	return true;
}

bool MediaDecoder::InternalOpen( const std::string &filename )
{
	bool useful_stream = false;
	if( avformat_open_input( &format_context, filename.c_str( ), nullptr, nullptr ) != 0 )
	{
		Close( );
		return false;
	}

	if( avformat_find_stream_info( format_context, nullptr ) < 0 )
	{
		Close( );
		return false;
	}

	for( uint32_t i = 0; i < format_context->nb_streams; i++ )
	{
		AVStream *stream = format_context->streams[i];
		AVCodecContext *codec_context = stream->codec;
		AVMediaType media_type = codec_context->codec_type;
		if( media_type == AVMEDIA_TYPE_AUDIO || media_type == AVMEDIA_TYPE_VIDEO )
		{
			AVCodec *codec = avcodec_find_decoder( codec_context->codec_id );
			if( codec == 0 )
				continue;

			if( avcodec_open2( codec_context, codec, 0 ) < 0 )
				continue;

			AVRational &time_base = stream->time_base;
			if( media_type == AVMEDIA_TYPE_AUDIO )
			{
				audio_channels = codec_context->channels;

				audio_samplerate = codec_context->sample_rate;

				audio_duration = std::chrono::microseconds( static_cast<int64_t>( stream->duration * av_q2d( time_base ) * 1000000 ) );

				uint64_t channel_layout = codec_context->channel_layout;
				if( channel_layout == 0 )
					channel_layout = av_get_default_channel_layout( codec_context->channels );

				swr_context = swr_alloc_set_opts( nullptr,
					channel_layout, AV_SAMPLE_FMT_S16, codec_context->sample_rate,
					channel_layout, codec_context->sample_fmt, codec_context->sample_rate,
					0, nullptr
				);

				swr_init( swr_context );
			}
			else if( media_type == AVMEDIA_TYPE_VIDEO )
			{
				video_width = codec_context->width;

				video_height = codec_context->height;

				video_duration = std::chrono::microseconds( static_cast<int64_t>( stream->duration * av_q2d( time_base ) * 1000000 ) );

				if( stream->avg_frame_rate.num && stream->avg_frame_rate.den )
					video_framerate = static_cast<float>( av_q2d( stream->avg_frame_rate ) );
				else
					video_framerate = 29.97f;

				sws_context = sws_getContext( 
					codec_context->width, codec_context->height, codec_context->pix_fmt,
					codec_context->width, codec_context->height, AV_PIX_FMT_RGBA,
					0, nullptr, nullptr, nullptr
				);

				sws_init_context( sws_context, nullptr, nullptr );
			}

			useful_stream = true;
		}
	}

	if( !useful_stream )
	{
		Close( );
		return false;
	}
	/*
	AVRational time_base;
	int64_t last_pts = 0;
	AVPacket packet;
	while( InternalRead( AVMEDIA_TYPE_UNKNOWN, &packet ) )
	{
	time_base = format_context->streams[packet.stream_index]->time_base;
	last_pts = packet.pts;
	av_free_packet( &packet );
	}
	// very accurate
	double duration = last_pts * av_q2d( time_base );
	*/
	return true;
}

bool MediaDecoder::InternalRead( StreamType type, AVPacket *packet )
{
	av_init_packet( packet );

	AVMediaType codectype = static_cast<AVMediaType>( type );
	AVStream **streams = format_context->streams;
	std::vector<AVPacket *>::iterator it = queued_packets.begin( );
	while( it != queued_packets.end( ) )
	{
		AVPacket *temp = *it;
		if( streams[temp->stream_index]->codec->codec_type == codectype )
		{
			*packet = *temp;
			queued_packets.erase( it );
			av_free( temp );
			return true;
		}

		++it;
	}

	AVPacket temp_packet;
	av_init_packet( &temp_packet );
	while( av_read_frame( format_context, &temp_packet ) >= 0 )
	{
		int index = temp_packet.stream_index;
		AVMediaType mtype = streams[index]->codec->codec_type;
		if( IsIgnoringStream( static_cast<StreamType>( mtype ) ) )
		{
			av_free_packet( &temp_packet );
			continue;
		}

		if( codectype == AVMEDIA_TYPE_UNKNOWN || mtype == codectype )
		{
			*packet = temp_packet;
			return true;
		}

		AVPacket *storage = static_cast<AVPacket *>( av_malloc( sizeof( AVPacket ) ) );
		av_copy_packet( storage, &temp_packet );
		av_free_packet( &temp_packet );
		queued_packets.push_back( storage );
	}

	return false;
}

int MediaDecoder::InternalMemoryRead( void *opaque, uint8_t *buf, int buf_size )
{
	MediaDecoder *decoder = static_cast<MediaDecoder *>( opaque );
	if( decoder->decoding_mode == DECODEMODE_MEMORY )
	{
		size_t remaining = static_cast<size_t>( decoder->memory_size - decoder->memory_pos );
		size_t copy_size = static_cast<size_t>( buf_size );
		if( remaining < copy_size )
			copy_size = remaining;
		std::memcpy( buf, &decoder->memory_buffer[decoder->memory_pos], copy_size );
		decoder->memory_pos += copy_size;
		return copy_size;
	}
	else if( decoder->decoding_mode == DECODEMODE_STREAM )
	{
		return static_cast<int>( decoder->input_stream->Read( buf, buf_size ) );
	}

	return -1;
}

int64_t MediaDecoder::InternalMemorySeek( void *opaque, int64_t offset, int whence )
{
	MediaDecoder *decoder = static_cast<MediaDecoder *>( opaque );
	if( decoder->decoding_mode == DECODEMODE_MEMORY )
	{
		switch( whence )
		{
		case SEEK_SET:
			return ( decoder->memory_pos = offset );

		case SEEK_CUR:
			return ( decoder->memory_pos += offset );

		case SEEK_END:
			return ( decoder->memory_pos = decoder->memory_size );

		case AVSEEK_SIZE:
			return decoder->memory_size;
		}
	}
	else if( decoder->decoding_mode == DECODEMODE_STREAM )
	{
		InputStream *stream = decoder->input_stream;
		if( whence == AVSEEK_SIZE )
		{
			return stream->Size( );
		}
		else
		{
			stream->Seek( offset, static_cast<SeekMode>( whence ) );
			return stream->Tell( );
		}
	}

	return -1;
}

void MediaDecoder::ResetPublisher( )
{
	if( input_stream != nullptr )
	{
		Close( );
		input_stream = nullptr;
	}
}

} // namespace MultiLibrary