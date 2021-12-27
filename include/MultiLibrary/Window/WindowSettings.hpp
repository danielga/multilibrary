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

#pragma once

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Window/Monitor.hpp>

namespace MultiLibrary
{

struct WindowSettings
{
public:
	WindowSettings(
		const Monitor &monitor = Monitor( ),
		bool visible = true,
		bool resizable = true,
		bool decorated = true,
		int32_t x = 0,
		int32_t y = 0,
		int32_t width = 0,
		int32_t height = 0,
		int32_t red_bits = 8,
		int32_t green_bits = 8,
		int32_t blue_bits = 8,
		int32_t alpha_bits = 8,
		int32_t depth_bits = 24,
		int32_t stencil_bits = 8,
		int32_t refresh_rate = 0,
		int32_t samples = 0,
		int32_t opengl_major = 1,
		int32_t opengl_minor = 0
	) :
		monitor( monitor ),
		visible( visible ),
		resizable( resizable ),
		decorated( decorated ),
		x( x ),
		y( y ),
		width( width ),
		height( height ),
		red_bits( red_bits ),
		green_bits( green_bits ),
		blue_bits( blue_bits ),
		alpha_bits( alpha_bits ),
		depth_bits( depth_bits ),
		stencil_bits( stencil_bits ),
		refresh_rate( refresh_rate ),
		samples( samples ),
		opengl_major( opengl_major ),
		opengl_minor( opengl_minor )
	{ }

	Monitor monitor;
	bool visible;
	bool resizable;
	bool decorated;
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
	int32_t red_bits;
	int32_t green_bits;
	int32_t blue_bits;
	int32_t alpha_bits;
	int32_t depth_bits;
	int32_t stencil_bits;
	int32_t refresh_rate;
	int32_t samples;
	int32_t opengl_major;
	int32_t opengl_minor;
};

} // namespace MultiLibrary
