#include <MultiLibrary/Media/OpenAL.hpp>
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace MultiLibrary
{

namespace Internal
{

void alCheckError( const std::string &file, unsigned int line )
{
	ALenum errorCode = alGetError( );
	if( errorCode != AL_NO_ERROR )
	{
		std::ostringstream error;
		error << "An internal OpenAL call failed in " << file << " (" << line << "): ";
		switch( errorCode )
		{
		case AL_INVALID_NAME:
			error << "AL_INVALID_NAME, an unacceptable name has been specified";
			break;

		case AL_INVALID_ENUM:
			error << "AL_INVALID_ENUM, an unacceptable value has been specified for an enumerated argument";
			break;

		case AL_INVALID_VALUE:
			error << "AL_INVALID_VALUE, a numeric argument is out of range";
			break;

		case AL_INVALID_OPERATION:
			error << "AL_INVALID_OPERATION, the specified operation is not allowed in the current state";
			break;

		case AL_OUT_OF_MEMORY:
			error << "AL_OUT_OF_MEMORY, there is not enough memory left to execute the command";
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