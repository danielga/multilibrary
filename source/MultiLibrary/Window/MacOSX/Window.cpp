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

#include <MultiLibrary/Window/Window.hpp>
#include <MultiLibrary/Window/MacOSX/Context.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <stdexcept>
#include <cstring>

namespace MultiLibrary
{

enum ButtonState
{
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_STICKY
};

namespace Internal
{

static const char window_class[] = "MultiLibrary";

} // namespace Internal

struct Window::Handle
{
	Handle( Display *display, ::Window handle, const WindowSettings &window_setup ) :
		window( handle ),
		context( display, handle, window_setup )
	{
		std::memset( keyboard_buttons, BUTTON_RELEASED, sizeof( keyboard_buttons ) );
		std::memset( mouse_buttons, BUTTON_RELEASED, sizeof( mouse_buttons ) );
	}

	::Window window;
	Context context;
	Monitor monitor;
	bool sticky_keys;
	bool sticky_mouse;
	ButtonState keyboard_buttons[KEY_CODE_LAST + 1];
	ButtonState mouse_buttons[MOUSE_BUTTON_LAST + 1];
};

Window::Window( ) :
	should_close( false )
{ }

Window::Window( const std::string &title, const WindowSettings &window_setup ) :
	should_close( false )
{
	Create( title, window_setup );
}

Window::~Window( )
{ }

bool Window::IsValid( ) const
{
	return static_cast<bool>( window_internal );
}

bool Window::Create( const std::string &title, const WindowSettings &window_setup )
{
	if( IsValid( ) )
		return false;

	glewExperimental = GL_TRUE;
	static GLenum err = glewInit( );
	if( err != GLEW_OK )
		throw std::runtime_error( "failed to initialize GLEW" );

	Display *display = XOpenDisplay( nullptr );
	if( display == nullptr )
		throw std::runtime_error( "unable to obtain X display" );

	XVisualInfo vinfo;
	if( XMatchVisualInfo(
		display,
		XDefaultScreen( display ),
		24,
		TrueColor,
		&vinfo
	) == 0 )
		throw std::runtime_error( "matching visual not found" );

	::Window window = XCreateWindow(
		display,
		XDefaultRootWindow( display ),
		0, 0,
		window_setup.width, window_setup.height,
		0,
		24,
		InputOutput,
		vinfo.visual,
		0,
		nullptr
	);
	if( window == None )
		throw std::runtime_error( "unable to create window" );

	if( XStoreName( display, window, title.c_str( ) ) == 0 )
		throw std::runtime_error( "failed to set window name" );

	window_internal = std::make_shared<Handle>( display, window, window_setup );
	window_internal->monitor = window_setup.monitor;

	return true;
}

void Window::Close( )
{
	if( !IsValid( ) )
		return;

	window_internal.reset( );
}

bool Window::ShouldClose( ) const
{
	if( !IsValid( ) )
		return false;

	return should_close;
}

void Window::SetShouldClose( bool close )
{
	if( !IsValid( ) )
		return;

	should_close = close;
}

bool Window::IsActive( ) const
{
	return window_internal->context.IsActive( );
}

void Window::SetActive( bool active )
{
	if( !IsValid( ) )
		return;

	window_internal->context.SetActive( active );
}

void Window::SetTitle( const std::string &title )
{
	if( !IsValid( ) )
		return;

	XStoreName( window_internal->context.display_handle, window_internal->window, title.c_str( ) );
}

void Window::SetIcon( uint32_t, uint32_t, const uint8_t * )
{

}

Vector2i Window::GetSize( ) const
{
	if( !IsValid( ) )
		return Vector2i( );

	unsigned int x = 0, y = 0;
	XGetGeometry( window_internal->context.display_handle, window_internal->window, nullptr, nullptr, nullptr, &x, &y, nullptr, nullptr );
	return Vector2i( x, y );
}

void Window::SetSize( const Vector2i &size )
{
	if( !IsValid( ) )
		return;

	XResizeWindow( window_internal->context.display_handle, window_internal->window, size.x, size.y );
}

Vector2i Window::GetFramebufferSize( ) const
{
	return GetSize( );
}

Vector2i Window::GetPosition( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	XGetGeometry( window_internal->context.display_handle, window_internal->window, nullptr, &pos.x, &pos.y, nullptr, nullptr, nullptr, nullptr );
	return pos;
}

void Window::SetPosition( const Vector2i &pos )
{
	if( !IsValid( ) )
		return;

	XMoveWindow( window_internal->context.display_handle, window_internal->window, pos.x, pos.y );
}

Vector2i Window::GetCursorPos( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	XQueryPointer( window_internal->context.display_handle, window_internal->window, nullptr, nullptr, nullptr, nullptr, &pos.x, &pos.y, nullptr );
	return pos;
}

void Window::SetCursorPos( const Vector2i &pos )
{
	if( !IsValid( ) )
		return;

	XWarpPointer( window_internal->context.display_handle, window_internal->window, None, 0, 0, 0, 0, pos.x, pos.y );
}

std::string Window::GetClipboardString( ) const
{
	return std::string( );
}

void Window::SetClipboardString( const std::string & )
{
	if( !IsValid( ) )
		return;


}

CursorMode Window::GetCursorMode( ) const
{
	if( !IsValid( ) )
		return static_cast<CursorMode>( -1 );

	return static_cast<CursorMode>( -1 );
}

void Window::SetCursorMode( CursorMode mode )
{
	if( !IsValid( ) )
		return;

	switch( mode )
	{
	case CURSOR_MODE_DISABLED:

		break;

	case CURSOR_MODE_HIDDEN:

		break;

	case CURSOR_MODE_NORMAL:

		break;
	}
}

bool Window::GetStickyKeysActive( ) const
{
	if( !IsValid( ) )
		return false;

	return window_internal->sticky_keys;
}

void Window::SetStickyKeysActive( bool active )
{
	if( !IsValid( ) )
		return;

	window_internal->sticky_keys = active;
}

bool Window::GetStickyMouseButtonsActive( ) const
{
	if( !IsValid( ) )
		return false;

	return window_internal->sticky_mouse;
}

void Window::SetStickyMouseButtonsActive( bool active )
{
	if( !IsValid( ) )
		return;

	window_internal->sticky_mouse = active;
}

bool Window::IsKeyPressed( KeyCode code ) const
{
	if( !IsValid( ) )
		return false;

	ButtonState state = window_internal->keyboard_buttons[code];
	if( state == BUTTON_STICKY )
		window_internal->keyboard_buttons[code] = BUTTON_RELEASED;

	return state != BUTTON_RELEASED;
}

bool Window::IsMouseButtonPressed( MouseButton code ) const
{
	if( !IsValid( ) )
		return false;

	ButtonState state = window_internal->mouse_buttons[code];
	if( state == BUTTON_STICKY )
		window_internal->mouse_buttons[code] = BUTTON_RELEASED;

	return state != BUTTON_RELEASED;
}

Monitor Window::GetFullscreenMonitor( ) const
{
	if( !IsValid( ) )
		return Monitor( );

	return window_internal->monitor;
}

bool Window::IsFocused( ) const
{
	if( !IsValid( ) )
		return false;

	return false;
}

bool Window::IsResizable( ) const
{
	if( !IsValid( ) )
		return false;

	return window_settings.resizable;
}

bool Window::IsDecorated( ) const
{
	if( !IsValid( ) )
		return false;

	return window_settings.decorated;
}

bool Window::IsIconified( ) const
{
	if( !IsValid( ) )
		return false;

	return false;
}

void Window::SetIconified( bool )
{
	if( !IsValid( ) )
		return;


}

bool Window::IsVisible( ) const
{
	if( !IsValid( ) )
		return false;

	return false;
}

void Window::SetVisible( bool )
{
	if( !IsValid( ) )
		return;


}

bool Window::SwapBuffers( )
{
	if( !IsValid( ) )
		return false;

	glXSwapBuffers( window_internal->context.display_handle, window_internal->context.window_handle );
	return true;
}

bool Window::SwapInterval( int32_t refreshes )
{
	if( !IsValid( ) && glXSwapIntervalEXT == nullptr )
		return false;

	glXSwapIntervalEXT( window_internal->context.display_handle, window_internal->context.window_handle, refreshes );
	return true;
}

void Window::PollEvents( )
{

}

void Window::WaitEvents( )
{

}

void Window::OnClose( ) { }

void Window::OnRefresh( ) { }

void Window::OnResize( const Vector2i & ) { }

void Window::OnReposition( const Vector2i & ) { }

void Window::OnFocus( bool ) { }

void Window::OnIconify( bool ) { }

void Window::OnFramebufferResize( const Vector2i & ) { }

void Window::OnKeyPress( int32_t, int32_t, int32_t, int32_t ) { }

void Window::OnCharacterInput( uint32_t ) { }

void Window::OnMousePress( int32_t, int32_t, int32_t ) { }

void Window::OnCursorReposition( const Vector2d & ) { }

void Window::OnCursorEnter( bool ) { }

void Window::OnScroll( const Vector2d & ) { }

} // namespace MultiLibrary
