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

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <MultiLibrary/Common/Pipe.hpp>
#include <string>
#include <vector>
#include <memory>

namespace MultiLibrary
{

/*!
 \brief A class that wraps the operating system's process functionality.
 */
class MULTILIBRARY_COMMON_API Process : public NonCopyable
{
public:
	enum class Status
	{
		Unknown = -1,
		Running,
		Terminated,
		Killed
	};

	/*!
	 \brief Constructor.

	 \param path Executable path.
	 \param args Arguments to send to the executable.
	 */
	Process( const std::string &path, const std::vector<std::string> &args = std::vector<std::string>( ) );

	/*!
	 \brief Destructor.

	 The internal process handles are also destroyed.
	 */
	~Process( );

	/*!
	 \brief Get process status.

	 \return status of the process
	 */
	Status GetStatus( ) const;

	/*!
	 \brief Close this process.

	 \return true if it succeeds, false if it fails.
	 */
	bool Close( );

	/*!
	 \brief Kill this process.

	 \deprecated This function is unsafe. Resources might leak.
	 */
	void Kill( );

	/*!
	 \brief Close the input pipe.
	 */
	void CloseInput( );

	/*!
	 \brief Get the input stream.

	 \return Input stream object.
	 */
	Pipe &Input( );

	/*!
	 \brief Get the output stream.

	 \return Output stream object.
	 */
	Pipe &Output( );

	/*!
	 \brief Get the error output stream.

	 \return Error output stream object.
	 */
	Pipe &Error( );

	/*!
	 \brief Return the process exit code.

	 \return Process exit code.
	 */
	int32_t ExitCode( ) const;

private:
	class Handle;
	std::unique_ptr<Handle> process;
	int32_t exit_code;
	Pipe input_pipe;
	Pipe output_pipe;
	Pipe error_pipe;
};

} // namespace MultiLibrary
