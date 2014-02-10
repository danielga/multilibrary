#pragma once

#include <MultiLibrary/Window/Export.hpp>

namespace MultiLibrary
{

struct ContextSettings
{
public:
	ContextSettings(
		int32_t samples = 1,
		int32_t alpha_bits = 8,
		int32_t depth_bits = 24,
		int32_t stencil_bits = 8,
		int32_t major_version = 1,
		int32_t minor_version = 0
	) :
		samples( samples ),
		alpha_bits( alpha_bits ),
		depth_bits( depth_bits ),
		stencil_bits( stencil_bits ),
		major_version( major_version ),
		minor_version( minor_version )
	{ }

	int32_t samples;
	int32_t alpha_bits;
	int32_t depth_bits;
	int32_t stencil_bits;
	int32_t major_version;
	int32_t minor_version;
};

} // namespace MultiLibrary