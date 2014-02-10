#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Common/BaseAttachable.hpp>
#include <MultiLibrary/Common/Time.hpp>
#include <string>
#include <vector>

namespace MultiLibrary
{

class InputStream;
class MediaDecoder;

class MULTILIBRARY_MEDIA_API SoundBuffer : public BaseAttachable
{
public:
	SoundBuffer( );
	~SoundBuffer( );

	bool Load( const std::string &filename );
	bool Load( const void *data, size_t bytes );
	bool Load( InputStream &stream );
	bool Load( const int16_t *samples, size_t samples_count, uint32_t channel_count, uint32_t samplerate );

	// Allows 3D spatialization (OpenAL only allows it on mono sounds)
	bool ConvertToMono( );

	const int16_t *GetSamples( ) const;
	size_t GetSampleCount( ) const;
	uint32_t GetSampleRate( ) const;
	uint32_t GetChannelCount( ) const;
	Time GetDuration( ) const;
	uint32_t GetBufferIndex( ) const;

private:
	bool Initialize( MediaDecoder &file );
	bool Update( uint32_t channels, uint32_t samplerate );

	std::vector<short> samples_buffer;
	uint32_t buffer_id;
	Time buffer_duration;
};

} // namespace MultiLibrary