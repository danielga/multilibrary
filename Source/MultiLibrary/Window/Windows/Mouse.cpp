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

#include <MultiLibrary/Window/Mouse.hpp>
#include <Windows.h>

namespace MultiLibrary
{

bool Mouse::IsButtonPressed( MouseButton button )
{
	int32_t vkey = 0;
	switch( button )
	{
		case MOUSE_BUTTON_LEFT:		vkey = GetSystemMetrics( SM_SWAPBUTTON ) != 0 ? VK_RBUTTON : VK_LBUTTON; break;
		case MOUSE_BUTTON_RIGHT:	vkey = GetSystemMetrics( SM_SWAPBUTTON ) != 0 ? VK_LBUTTON : VK_RBUTTON; break;
		case MOUSE_BUTTON_MIDDLE:	vkey = VK_MBUTTON; break;
		case MOUSE_BUTTON_4:		vkey = VK_XBUTTON1; break;
		case MOUSE_BUTTON_5:		vkey = VK_XBUTTON2; break;
		default:					return false;
	}

	return ( GetAsyncKeyState( vkey ) & 0x8000 ) != 0;
}

Vector2i Mouse::GetPosition( )
{
	POINT point;
	if( ::GetCursorPos( &point ) == FALSE )
		return Vector2i( );

	return Vector2i( point.x, point.y );
}

void Mouse::SetPosition( const Vector2i &pos )
{
	SetCursorPos( pos.x, pos.y );
}

} // namespace MultiLibrary