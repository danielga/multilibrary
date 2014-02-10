#pragma once

#include <MultiLibrary/Window/Export.hpp>

namespace MultiLibrary
{

struct VideoMode
{
public:
	VideoMode(
		int32_t width = 0,
		int32_t height = 0,
		int32_t red_bits = 8,
		int32_t green_bits = 8,
		int32_t blue_bits = 8,
		int32_t refresh_rate = 0
	) :
		width( width ),
		height( height ),
		red_bits( red_bits ),
		green_bits( green_bits ),
		blue_bits( blue_bits ),
		refresh_rate( refresh_rate )
	{ }

	int32_t width;
	int32_t height;
	int32_t red_bits;
	int32_t green_bits;
	int32_t blue_bits;
	int32_t refresh_rate;
};

} // namespace MultiLibrary