/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
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

#include <MultiLibrary/Common/Export.hpp>

namespace MultiLibrary
{

/*!
 \brief A class that makes any derived class non copyable.
 */
class MULTILIBRARY_COMMON_API NonCopyable
{
protected:
	/*!
	 \brief Default constructor.

	 Because this class has a copy constructor, the compiler
	 will not automatically generate the default constructor.
	 That's why we must define it explicitely.
	 */
	NonCopyable( ) { }

private:
	/*!
	 \brief Copy constructor.

	 Because this constructor was made private, classes that try to use it,
	 will be met with a compilation error. It also doesn't have any
	 definition so if the first protection fails, a linker error occurs.
	 */
	NonCopyable( const NonCopyable & ) = delete;

	/*!
	 \brief Assignment operator.

	 Because this assignment operator was made private, classes that try to
	 use it, will be met with a compilation error. It also doesn't have any
	 definition so if the first protection fails, a linker error occurs.
	 */
	NonCopyable &operator=( const NonCopyable & ) = delete;
};

} // namespace MultiLibrary
