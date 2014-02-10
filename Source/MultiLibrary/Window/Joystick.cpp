#include <MultiLibrary/Window/Joystick.hpp>
#include <MultiLibrary/Window/GLFW.hpp>

namespace MultiLibrary
{

Joystick::Joystick( JoystickNumber joystick ) :
	joystick_number( joystick )
{ }

bool Joystick::IsValid( ) const
{
	return glfwJoystickPresent( joystick_number ) == GL_TRUE;
}

std::string Joystick::GetName( ) const
{
	if( !IsValid( ) )
		return std::string( );

	return glfwGetJoystickName( joystick_number );
}

std::vector<float> Joystick::GetAxes( ) const
{
	std::vector<float> axes;
	if( !IsValid( ) )
		return axes;

	int32_t c = 0;
	for( const float *a = glfwGetJoystickAxes( joystick_number, &c ), *e = a + c; a != e; ++a )
		axes.push_back( *a );

	return axes;
}

std::vector<uint8_t> Joystick::GetButtons( ) const
{
	std::vector<uint8_t> buttons;
	if( !IsValid( ) )
		return buttons;

	int32_t c = 0;
	for( const uint8_t *b = glfwGetJoystickButtons( joystick_number, &c ), *e = b + c; b != e; ++b )
		buttons.push_back( *b );

	return buttons;
}

} // namespace MultiLibrary