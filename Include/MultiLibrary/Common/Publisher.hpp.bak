/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
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

class Subscriber;

/*!
 \brief An interface for objects that can receive subscribers.

 The general idea of this is that if you want to use an object and you
 have no idea when it'll be destroyed, you subscribe to it and it'll
 tell you when it's being destroyed. This prevents crashes caused by,
 for example, streaming an audio file through an object and that object
 is suddenly destroyed.
 Of course, if you properly code your program, you'll never have to use
 this.
 */
class Publisher
{
public:
	/*!
	 \brief Attach to an anchor.

	 This is called by anchors to warn we've been referenced.

	 \param base Anchor to attach to.
	 */
	virtual void Subscribe( Subscriber *base ) = 0;

	/*!
	 \brief Detach from an anchor.

	 This is called by anchors to warn we've been dereferenced.

	 \param base Anchor to detach from.
	 */
	virtual void Unsubscribe( Subscriber *base ) = 0;
};

} // namespace MultiLibrary