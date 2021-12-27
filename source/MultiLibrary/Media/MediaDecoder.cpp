/*************************************************************************
 * MultiLibrary - https://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2022, Daniel Almeida
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
 *************************************************************************/

#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <MultiLibrary/Common/InputStream.hpp>
#include <cstring>

#ifdef _WIN32

/// Disables "The enum type is unscoped. Prefer 'enum class' over 'enum'." warnings.
#pragma warning( disable : 26812 )

#pragma warning( push )
/// Disables "'return': conversion from 'int' to 'X', possible loss of data" warnings.
#pragma warning( disable : 4244 )

#endif

extern "C"
{
	#include <libavformat/avformat.h>
	#include <libswresample/swresample.h>
	#include <libswscale/swscale.h>
	#include <libavutil/pixdesc.h>
	#include <libavutil/channel_layout.h>
}

#ifdef _WIN32

#pragma warning( pop )

#endif

using namespace std::chrono_literals;

namespace MultiLibrary
{

MediaDecoder::MediaDecoder( ) :
	format_context( nullptr ),
	decoding_mode( DecodeMode::None ),
	memory_buffer( nullptr ),
	memory_size( 0 ),
	memory_pos( 0 ),
	temp_memory( nullptr, av_free ),
	input_stream( nullptr ),
	audio_channels( 0 ),
	audio_samplerate( 0 ),
	audio_duration( 0us ),
	video_width( 0 ),
	video_height( 0 ),
	video_framerate( 0.0f ),
	video_duration( 0us ),
	swr_context( nullptr ),
	sws_context( nullptr )
{
	for( int32_t k = 0; k < static_cast<int32_t>( StreamType::Count ); ++k )
		ignored_streams[k] = false;

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)

	av_register_all( );

#endif

}

MediaDecoder::~MediaDecoder( )
{
	Close( );
}

bool MediaDecoder::Open( const std::string &filename )
{
	if( IsOpen( ) )
		return false;

	decoding_mode = DecodeMode::File;
	return InternalOpen( filename );
}

bool MediaDecoder::Open( const void *buffer, size_t size )
{
	if( IsOpen( ) )
		return false;

	decoding_mode = DecodeMode::Memory;
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

	decoding_mode = DecodeMode::Stream;

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

	for( AVCodecContext *codec_context : stream_codecs )
	{
		if( codec_context != nullptr && codec_context->codec != nullptr )
			avcodec_free_context( &codec_context );
	}

	stream_codecs.clear( );

	if( swr_context != nullptr )
		swr_free( &swr_context );

	if( sws_context != nullptr )
	{
		sws_freeContext( sws_context );
		sws_context = nullptr;
	}

	if( decoding_mode == DecodeMode::File )
	{
		avformat_close_input( &format_context );
	}
	else if( decoding_mode != DecodeMode::None )
	{
		av_free( format_context->pb );
		format_context->pb = nullptr;

		avformat_free_context( format_context );
		format_context = nullptr;
	}

	if( input_stream != nullptr )
		input_stream = nullptr;

	decoding_mode = DecodeMode::None;
	memory_buffer = nullptr;
	memory_size = 0;
	memory_pos = 0;

	audio_channels = 0;
	audio_samplerate = 0;
	audio_duration = 0us;

	video_width = 0;
	video_height = 0;
	video_framerate = 0.0f;
	video_duration = 0us;

	return true;
}

void MediaDecoder::ClearQueue( )
{
	std::vector<AVPacket *>::iterator it = queued_packets.begin( );
	while( it != queued_packets.end( ) )
	{
		AVPacket *temp = *it;
		av_packet_free( &temp );

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
	int32_t channels = frame->channels;
	int32_t nbsamples = frame->nb_samples;

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

	int32_t len = 0;
	int32_t done = 0;
	AVPacket *packet = nullptr;
	while( done == 0 && ( packet = InternalRead( StreamType::Audio ) ) != nullptr )
	{
		uint8_t *data = packet->data;
		int32_t size = packet->size;
		while( packet->size > 0 )
		{
			if( packet->pos == -1 )
			{
				// packet is useless and so skip it
				packet->size = 0;
			}
			else
			{
				if( avcodec_send_packet( stream_codecs[packet->stream_index], packet ) != 0 )
					break;

				if( avcodec_receive_frame( stream_codecs[packet->stream_index], frame ) != 0 )
					break;

				len += frame->pkt_size;

				if( len <= 0 )
					break;

				done += len;

				packet->data += len;
				packet->size -= len;
			}
		}

		packet->data = data;
		packet->size = size;
		av_packet_free( &packet );
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
	int32_t planes = av_pix_fmt_count_planes( format );
	int32_t width = frame->width;
	int32_t height = frame->height;

	video_frame.data.resize( width * height * planes );

	uint8_t *data[] = { &video_frame.data[0] };

	int32_t strides[] = { planes * width, 0, 0, 0 };
	sws_scale( sws, frame->extended_data, frame->linesize, 0, height, data, strides );

	video_frame.width = width;
	video_frame.height = height;
}

bool MediaDecoder::ReadVideo( VideoFrame &video_frame )
{
	if( !IsOpen( ) )
		return false;

	AVFrame *frame = av_frame_alloc( );

	int32_t len = 0;
	int32_t done = 0;
	AVPacket *packet = nullptr;
	while( done == 0 && ( packet = InternalRead( StreamType::Video ) ) != nullptr )
	{
		uint8_t *data = packet->data;
		int32_t size = packet->size;
		while( packet->size > 0 )
		{
			if( packet->pos == -1 )
			{
				// packet is useless and so skip it
				packet->size = 0;
			}
			else
			{
				if( avcodec_send_packet( stream_codecs[packet->stream_index], packet ) != 0 )
					break;

				if( avcodec_receive_frame( stream_codecs[packet->stream_index], frame ) != 0 )
					break;

				len += frame->pkt_size;

				if( len <= 0 )
					break;

				done += len;

				packet->data += len;
				packet->size -= len;
			}
		}

		packet->data = data;
		packet->size = size;
		av_packet_free( &packet );
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

int64_t MediaDecoder::GetPosition( )
{
	if( !IsOpen( ) )
		return 0;

	return 0;
}

void MediaDecoder::IgnoreStream( StreamType stream_type, bool ignore )
{
	if( stream_type >= StreamType::Video && stream_type < StreamType::Count )
		ignored_streams[static_cast<size_t>( stream_type )] = ignore;
}

bool MediaDecoder::IsIgnoringStream( StreamType stream_type )
{
	if( stream_type >= StreamType::Video && stream_type < StreamType::Count )
		return ignored_streams[static_cast<size_t>( stream_type )];

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

	stream_codecs.resize( format_context->nb_streams );
	for( uint32_t i = 0; i < format_context->nb_streams; ++i )
	{
		AVStream *stream = format_context->streams[i];
		AVCodecParameters *codec_parameters = stream->codecpar;
		AVMediaType media_type = codec_parameters->codec_type;

		if( media_type == AVMEDIA_TYPE_AUDIO || media_type == AVMEDIA_TYPE_VIDEO )
		{
			AVCodec *codec = avcodec_find_decoder( codec_parameters->codec_id );
			if( codec == nullptr )
				continue;

			stream_codecs[i] = avcodec_alloc_context3( codec );
			if( stream_codecs[i] == nullptr )
				continue;

			if( avcodec_open2( stream_codecs[i], codec, nullptr ) < 0 )
			{
				avcodec_free_context( &stream_codecs[i] );
				continue;
			}

			AVRational &time_base = stream->time_base;
			if( media_type == AVMEDIA_TYPE_AUDIO )
			{
				audio_channels = codec_parameters->channels;

				audio_samplerate = codec_parameters->sample_rate;

				auto duration = std::chrono::seconds( static_cast<int64_t>( stream->duration * av_q2d( time_base ) ) );
				audio_duration = std::chrono::duration_cast<std::chrono::microseconds>( duration );

				uint64_t channel_layout = codec_parameters->channel_layout;
				if( channel_layout == 0 )
					channel_layout = av_get_default_channel_layout( codec_parameters->channels );

				swr_context = swr_alloc_set_opts( nullptr,
					channel_layout, AV_SAMPLE_FMT_S16, codec_parameters->sample_rate,
					channel_layout, static_cast<AVSampleFormat>( codec_parameters->format ),
					codec_parameters->sample_rate, 0, nullptr
				);

				swr_init( swr_context );
			}
			else if( media_type == AVMEDIA_TYPE_VIDEO )
			{
				video_width = codec_parameters->width;

				video_height = codec_parameters->height;

				auto duration = std::chrono::seconds( static_cast<int64_t>( stream->duration * av_q2d( time_base ) ) );
				video_duration = std::chrono::duration_cast<std::chrono::microseconds>( duration );

				if( stream->avg_frame_rate.num && stream->avg_frame_rate.den )
					video_framerate = static_cast<float>( av_q2d( stream->avg_frame_rate ) );
				else
					video_framerate = 29.97f;

				sws_context = sws_getContext( 
					codec_parameters->width, codec_parameters->height,
					static_cast<AVPixelFormat>( codec_parameters->format ),
					codec_parameters->width, codec_parameters->height, AV_PIX_FMT_RGBA,
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
	AVRational time_base = { 0, 0 };
	int64_t last_pts = 0;
	AVPacket *packet = nullptr;
	while( ( packet = InternalRead( StreamType::Audio ) ) != nullptr )
	{
		time_base = format_context->streams[packet->stream_index]->time_base;
		last_pts = packet->pts;
		av_packet_unref( packet );
	}

	// very accurate
	const double duration = last_pts * av_q2d( time_base );
	printf( "Media duration: %fs\n", duration );
	*/

	return true;
}

AVPacket *MediaDecoder::InternalRead( StreamType type )
{
	AVMediaType codectype = static_cast<AVMediaType>( type );
	AVStream **streams = format_context->streams;
	auto it = queued_packets.begin( );
	while( it != queued_packets.end( ) )
	{
		AVPacket *temp = *it;
		if( streams[temp->stream_index]->codecpar->codec_type == codectype )
		{
			queued_packets.erase( it );
			return temp;
		}

		++it;
	}

	AVPacket *temp_packet = av_packet_alloc( );
	while( av_read_frame( format_context, temp_packet ) >= 0 )
	{
		int32_t index = temp_packet->stream_index;
		AVMediaType mtype = streams[index]->codecpar->codec_type;
		if( IsIgnoringStream( static_cast<StreamType>( mtype ) ) )
		{
			av_packet_unref( temp_packet );
			continue;
		}

		if( codectype == AVMEDIA_TYPE_UNKNOWN || mtype == codectype )
			return temp_packet;

		queued_packets.push_back( temp_packet );
		temp_packet = av_packet_alloc( );
	}

	av_packet_free( &temp_packet );
	return nullptr;
}

int32_t MediaDecoder::InternalMemoryRead( void *opaque, uint8_t *buf, int32_t buf_size )
{
	MediaDecoder *decoder = static_cast<MediaDecoder *>( opaque );
	if( decoder->decoding_mode == DecodeMode::Memory )
	{
		size_t remaining = static_cast<size_t>( decoder->memory_size - decoder->memory_pos );
		size_t copy_size = static_cast<size_t>( buf_size );
		if( remaining < copy_size )
			copy_size = remaining;
		std::memcpy( buf, &decoder->memory_buffer[decoder->memory_pos], copy_size );
		decoder->memory_pos += copy_size;
		return static_cast<int32_t>( copy_size );
	}
	else if( decoder->decoding_mode == DecodeMode::Stream )
	{
		return static_cast<int32_t>( decoder->input_stream->Read( buf, buf_size ) );
	}

	return -1;
}

int64_t MediaDecoder::InternalMemorySeek( void *opaque, int64_t offset, int32_t whence )
{
	MediaDecoder *decoder = static_cast<MediaDecoder *>( opaque );
	if( decoder->decoding_mode == DecodeMode::Memory )
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
	else if( decoder->decoding_mode == DecodeMode::Stream )
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
