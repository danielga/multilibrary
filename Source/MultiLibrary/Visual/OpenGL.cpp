#include <MultiLibrary/Visual/OpenGL.hpp>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace MultiLibrary
{

namespace Internal
{

void glCheckError( const std::string &file, uint32_t line )
{
	GLenum errorCode = glGetError( );
	if( errorCode != GL_NO_ERROR )
	{
		std::ostringstream error;
		error << "An internal OpenGL call failed in " << file << " (" << line << "): ";

		switch( errorCode )
		{
		case GL_INVALID_ENUM:
			error << "GL_INVALID_ENUM, an unacceptable value has been specified for an enumerated argument";
			break;

		case GL_INVALID_VALUE:
			error << "GL_INVALID_VALUE, a numeric argument is out of range";
			break;

		case GL_INVALID_OPERATION:
			error << "GL_INVALID_OPERATION, the specified operation is not allowed in the current state";
			break;

		case GL_STACK_OVERFLOW:
			error << "GL_STACK_OVERFLOW, this command would cause a stack overflow";
			break;

		case GL_STACK_UNDERFLOW:
			error << "GL_STACK_UNDERFLOW, this command would cause a stack underflow";
			break;

		case GL_OUT_OF_MEMORY:
			error << "GL_OUT_OF_MEMORY, there is not enough memory left to execute the command";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
			error << "GL_INVALID_FRAMEBUFFER_OPERATION_EXT, the object bound to FRAMEBUFFER_BINDING_EXT is not \"framebuffer complete\"";
			break;

		default:
			error << "unknown error code " << "0x" << std::hex << std::setw( 8 ) << std::setfill( '0' ) << errorCode;
			break;
		}

		throw std::runtime_error( error.str( ) );
	}
}

} // namespace Internal

} // namespace MultiLibrary