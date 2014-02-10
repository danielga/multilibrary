#pragma once

#include <MultiLibrary/Media/Export.hpp>
#include <string>
#include <vector>

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

namespace MultiLibrary
{

class MULTILIBRARY_MEDIA_API AudioDevice
{
public:
	AudioDevice( );
	AudioDevice( const std::string &devname );
	~AudioDevice( );

	void OpenDevice( );
	void OpenDevice( const std::string &devname );
	void CloseDevice( );

	void MakeCurrent( );

	static std::vector<std::string> GetAudioDevices( );
	static int GetFormatFromChannelCount( uint32_t numChannels );

private:
	ALCdevice *openal_device;
	ALCcontext *openal_context;
};

} // namespace MultiLibrary