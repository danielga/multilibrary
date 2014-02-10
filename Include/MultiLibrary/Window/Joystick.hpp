#pragma once

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
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