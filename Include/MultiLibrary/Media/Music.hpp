#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <MultiLibrary/Media/SoundStream.hpp>
#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <vector>

namespace MultiLibrary
{

class MULTILIBRARY_MEDIA_API Music : public SoundStream
{
public:
	Music( );
	virtual ~Music( );

	bool Open( const std::string &filename );
	bool Open( const void *data, size_t data_size );
	bool Open( class InputStream &stream );

protected:
	bool GetData( std::vector<int16_t> &buffer, size_t min );
	bool Seek( const Time &time_offset );

private:
	MediaDecoder media_decoder;
};

} // namespace MultiLibrary