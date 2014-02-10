#include <MultiLibrary/Window/GLFW.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace MultiLibrary
{

namespace Internal
{

class GLFWInitializer
{
public:
	GLFWInitializer( )
	{
		if( glfwInit( ) == GL_FALSE )
			throw std::runtime_error( "unable to initialize glfw" );

		glfwSetErrorCallback( &ErrorCallback );
	}

	~GLFWInitializer( )
	{
		glfwTerminate( );
	}

	static void ErrorCallback( int errcode, const char *strerr )
	{
		std::ostringstream error;
		error << "An internal GLFW call failed with error code ";
		error << "0x" << std::hex << std::setw( 8 ) << std::setfill( '0' ) << errcode << ": " << strerr;
		throw std::runtime_error( error.str( ) );
	}
};

void InitializeGLFW( )
{
	static Internal::GLFWInitializer __glfw_init;
}

} // namespace Internal

} // namespace MultiLibrary