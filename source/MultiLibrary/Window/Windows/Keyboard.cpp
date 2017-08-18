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

#include <MultiLibrary/Window/Keyboard.hpp>
#include <Windows.h>

namespace MultiLibrary
{

bool Keyboard::IsButtonPressed( KeyCode code )
{
	int32_t vkey = 0;
	switch( code )
	{
		case KEY_CODE_BACKSPACE:
			vkey = VK_BACK;
			break;

		case KEY_CODE_TAB:
			vkey = VK_TAB;
			break;

		case KEY_CODE_ENTER:
			vkey = VK_RETURN;
			break;

		case KEY_CODE_PAUSE:
			vkey = VK_PAUSE;
			break;

		case KEY_CODE_ESCAPE:
			vkey = VK_ESCAPE;
			break;

		case KEY_CODE_SPACE:
			vkey = VK_SPACE;
			break;

		case KEY_CODE_PAGE_UP:
			vkey = VK_PRIOR;
			break;

		case KEY_CODE_PAGE_DOWN:
			vkey = VK_NEXT;
			break;

		case KEY_CODE_END:
			vkey = VK_END;
			break;

		case KEY_CODE_HOME:
			vkey = VK_HOME;
			break;

		case KEY_CODE_LEFT:
			vkey = VK_LEFT;
			break;

		case KEY_CODE_UP:
			vkey = VK_UP;
			break;

		case KEY_CODE_RIGHT:
			vkey = VK_RIGHT;
			break;

		case KEY_CODE_DOWN:
			vkey = VK_DOWN;
			break;

		case KEY_CODE_INSERT:
			vkey = VK_INSERT;
			break;

		case KEY_CODE_DELETE:
			vkey = VK_DELETE;
			break;

		case KEY_CODE_A:
			vkey = 'A';
			break;

		case KEY_CODE_B:
			vkey = 'B';
			break;

		case KEY_CODE_C:
			vkey = 'C';
			break;

		case KEY_CODE_D:
			vkey = 'D';
			break;

		case KEY_CODE_E:
			vkey = 'E';
			break;

		case KEY_CODE_F:
			vkey = 'F';
			break;

		case KEY_CODE_G:
			vkey = 'G';
			break;

		case KEY_CODE_H:
			vkey = 'H';
			break;

		case KEY_CODE_I:
			vkey = 'I';
			break;

		case KEY_CODE_J:
			vkey = 'J';
			break;

		case KEY_CODE_K:
			vkey = 'K';
			break;

		case KEY_CODE_L:
			vkey = 'L';
			break;

		case KEY_CODE_M:
			vkey = 'M';
			break;

		case KEY_CODE_N:
			vkey = 'N';
			break;

		case KEY_CODE_O:
			vkey = 'O';
			break;

		case KEY_CODE_P:
			vkey = 'P';
			break;

		case KEY_CODE_Q:
			vkey = 'Q';
			break;

		case KEY_CODE_R:
			vkey = 'R';
			break;

		case KEY_CODE_S:
			vkey = 'S';
			break;

		case KEY_CODE_T:
			vkey = 'T';
			break;

		case KEY_CODE_U:
			vkey = 'U';
			break;

		case KEY_CODE_V:
			vkey = 'V';
			break;

		case KEY_CODE_W:
			vkey = 'W';
			break;

		case KEY_CODE_X:
			vkey = 'X';
			break;

		case KEY_CODE_Y:
			vkey = 'Y';
			break;

		case KEY_CODE_Z:
			vkey = 'Z';
			break;

		case KEY_CODE_0:
			vkey = '0';
			break;

		case KEY_CODE_1:
			vkey = '1';
			break;

		case KEY_CODE_2:
			vkey = '2';
			break;

		case KEY_CODE_3:
			vkey = '3';
			break;

		case KEY_CODE_4:
			vkey = '4';
			break;

		case KEY_CODE_5:
			vkey = '5';
			break;

		case KEY_CODE_6:
			vkey = '6';
			break;

		case KEY_CODE_7:
			vkey = '7';
			break;

		case KEY_CODE_8:
			vkey = '8';
			break;

		case KEY_CODE_9:
			vkey = '9';
			break;

		case KEY_CODE_LEFT_SUPER:
			vkey = VK_LWIN;
			break;

		case KEY_CODE_RIGHT_SUPER:
			vkey = VK_RWIN;
			break;

		case KEY_CODE_MENU:
			vkey = VK_APPS;
			break;

		case KEY_CODE_KP_0:
			vkey = VK_NUMPAD0;
			break;

		case KEY_CODE_KP_1:
			vkey = VK_NUMPAD1;
			break;

		case KEY_CODE_KP_2:
			vkey = VK_NUMPAD2;
			break;

		case KEY_CODE_KP_3:
			vkey = VK_NUMPAD3;
			break;

		case KEY_CODE_KP_4:
			vkey = VK_NUMPAD4;
			break;

		case KEY_CODE_KP_5:
			vkey = VK_NUMPAD5;
			break;

		case KEY_CODE_KP_6:
			vkey = VK_NUMPAD6;
			break;

		case KEY_CODE_KP_7:
			vkey = VK_NUMPAD7;
			break;

		case KEY_CODE_KP_8:
			vkey = VK_NUMPAD8;
			break;

		case KEY_CODE_KP_9:
			vkey = VK_NUMPAD9;
			break;

		case KEY_CODE_KP_MULTIPLY:
			vkey = VK_MULTIPLY;
			break;

		case KEY_CODE_KP_ADD:
			vkey = VK_ADD;
			break;

		case KEY_CODE_KP_EQUAL:
		case KEY_CODE_KP_ENTER:
			vkey = VK_SEPARATOR;
			break;

		case KEY_CODE_KP_SUBTRACT:
			vkey = VK_SUBTRACT;
			break;

		case KEY_CODE_KP_DECIMAL:
			vkey = VK_DECIMAL;
			break;

		case KEY_CODE_KP_DIVIDE:
			vkey = VK_DIVIDE;
			break;

		case KEY_CODE_F1:
			vkey = VK_F1;
			break;

		case KEY_CODE_F2:
			vkey = VK_F2;
			break;

		case KEY_CODE_F3:
			vkey = VK_F3;
			break;

		case KEY_CODE_F4:
			vkey = VK_F4;
			break;

		case KEY_CODE_F5:
			vkey = VK_F5;
			break;

		case KEY_CODE_F6:
			vkey = VK_F6;
			break;

		case KEY_CODE_F7:
			vkey = VK_F7;
			break;

		case KEY_CODE_F8:
			vkey = VK_F8;
			break;

		case KEY_CODE_F9:
			vkey = VK_F9;
			break;

		case KEY_CODE_F10:
			vkey = VK_F10;
			break;

		case KEY_CODE_F11:
			vkey = VK_F11;
			break;

		case KEY_CODE_F12:
			vkey = VK_F12;
			break;

		case KEY_CODE_F13:
			vkey = VK_F13;
			break;

		case KEY_CODE_F14:
			vkey = VK_F14;
			break;

		case KEY_CODE_F15:
			vkey = VK_F15;
			break;

		case KEY_CODE_F16:
			vkey = VK_F16;
			break;

		case KEY_CODE_F17:
			vkey = VK_F17;
			break;

		case KEY_CODE_F18:
			vkey = VK_F18;
			break;

		case KEY_CODE_F19:
			vkey = VK_F19;
			break;

		case KEY_CODE_F20:
			vkey = VK_F20;
			break;

		case KEY_CODE_F21:
			vkey = VK_F21;
			break;

		case KEY_CODE_F22:
			vkey = VK_F22;
			break;

		case KEY_CODE_F23:
			vkey = VK_F23;
			break;

		case KEY_CODE_F24:
			vkey = VK_F24;
			break;

		case KEY_CODE_LEFT_SHIFT:
			vkey = VK_LSHIFT;
			break;

		case KEY_CODE_RIGHT_SHIFT:
			vkey = VK_RSHIFT;
			break;

		case KEY_CODE_LEFT_CONTROL:
			vkey = VK_LCONTROL;
			break;

		case KEY_CODE_RIGHT_CONTROL:
			vkey = VK_RCONTROL;
			break;

		case KEY_CODE_LEFT_ALT:
			vkey = VK_LMENU;
			break;

		case KEY_CODE_RIGHT_ALT:
			vkey = VK_RMENU;
			break;

		case KEY_CODE_EQUAL:
			vkey = VK_OEM_PLUS;
			break;

		case KEY_CODE_COMMA:
			vkey = VK_OEM_COMMA;
			break;

		case KEY_CODE_PERIOD:
			vkey = VK_OEM_PERIOD;
			break;

		case KEY_CODE_MINUS:
			vkey = VK_OEM_MINUS;
			break;

		case KEY_CODE_SEMICOLON:
			vkey = VK_OEM_1;
			break;

		case KEY_CODE_SLASH:
			vkey = VK_OEM_2;
			break;

		case KEY_CODE_GRAVE_ACCENT:
			vkey = VK_OEM_3;
			break;

		case KEY_CODE_LEFT_BRACKET:
			vkey = VK_OEM_4;
			break;

		case KEY_CODE_BACKSLASH:
			vkey = VK_OEM_5;
			break;

		case KEY_CODE_RIGHT_BRACKET:
			vkey = VK_OEM_6;
			break;

		case KEY_CODE_APOSTROPHE:
			vkey = VK_OEM_7;
			break;

		case KEY_CODE_WORLD_1:
			vkey = VK_OEM_8;
			break;

		case KEY_CODE_WORLD_2:
			vkey = VK_OEM_102;
			break;

		default:
			return false;
	}

	return ( GetAsyncKeyState( vkey ) & 0x8000 ) != 0;
}

} // namespace MultiLibrary
