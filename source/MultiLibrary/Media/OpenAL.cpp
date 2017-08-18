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
