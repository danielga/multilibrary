/*************************************************************************
 * MultiLibrary - http://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2017, Daniel Almeida
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

#include <MultiLibrary/Window/Export.hpp>
#include <string>
#include <vector>

namespace MultiLibrary
{

enum JoystickNumber
{
	JOYSTICK_1,
	JOYSTICK_2,
	JOYSTICK_3,
	JOYSTICK_4,
	JOYSTICK_5,
	JOYSTICK_6,
	JOYSTICK_7,
	JOYSTICK_8,
	JOYSTICK_9,
	JOYSTICK_10,
	JOYSTICK_11,
	JOYSTICK_12,
	JOYSTICK_13,
	JOYSTICK_14,
	JOYSTICK_15,
	JOYSTICK_16
};

class MULTILIBRARY_WINDOW_API Joystick
{
public:
	Joystick( JoystickNumber joystick );

	bool IsValid( ) const;

	std::string GetName( ) const;

	std::vector<float> GetAxes( ) const;

	std::vector<uint8_t> GetButtons( ) const;

private:
	JoystickNumber joystick_number;
};

} // namespace MultiLibrary
