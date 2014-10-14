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

#include <MultiLibrary/Window/Window.hpp>
#include <MultiLibrary/Window/Windows/Context.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <stdexcept>
#include <cstring>
#include <Windows.h>

#undef CreateWindow

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

static const wchar_t window_class[] = L"MultiLibrary";

class WindowInitializer
{
public:
	WindowInitializer( ) :
		class_atom( nullptr )
	{
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProcedure;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof( void * ) + sizeof( int32_t );
		wc.hInstance = GetModuleHandle( nullptr );
		wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = window_class;
		wc.hIcon = LoadIcon( nullptr, IDI_WINLOGO );
		ATOM atom = RegisterClass( &wc );
		if( atom == 0 )
			throw std::runtime_error( "failed to create window class" );

		class_atom = reinterpret_cast<LPCWSTR>( MAKELONG( atom, 0 ) );

		HWND temp_window = CreateWindowEx( 0, L"STATIC", L"", WS_POPUP | WS_DISABLED, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle( nullptr ), nullptr );
		if( temp_window == nullptr )
			throw std::runtime_error( "failed to create temporary window" );

		{
			Context context( temp_window, WindowSettings( ) );
			context.SetActive( true );

			if( glewInit( ) != GLEW_OK )
				throw new std::runtime_error( "unable to initialize glew" );
		}

		DestroyWindow( temp_window );
	}

	~WindowInitializer( )
	{
		UnregisterClass( class_atom, GetModuleHandle( nullptr ) );
	}

	LPCWSTR GetWindowClass( )
	{
		return class_atom;
	}

private:
	static LRESULT CALLBACK WindowProcedure( HWND window, UINT msg, WPARAM wparam, LPARAM lparam )
	{
		switch( msg )
		{
			case WM_NCCREATE:
			{
				LPCREATESTRUCT create_struct = reinterpret_cast<LPCREATESTRUCT>( lparam );
				SetWindowLongPtr( window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( create_struct->lpCreateParams ) );
				break;
			}

			case WM_CLOSE:
				reinterpret_cast<Window *>( GetWindowLongPtr( window, GWLP_USERDATA ) )->SetShouldClose( true );
				return 0;
		}

		return DefWindowProc( window, msg, wparam, lparam );
	}

	LPCWSTR class_atom;
};

static void GetFullWindowSize( DWORD flags, DWORD exflags, int32_t win, int32_t hin, int32_t &wout, int32_t &hout )
{
	RECT rect = { 0, 0, win, hin };
	if( AdjustWindowRectEx( &rect, flags, FALSE, exflags ) == FALSE )
		return;

	wout = rect.right - rect.left;
	hout = rect.bottom - rect.top;
}

} // namespace Internal

struct Window::Handle
{
	Handle( HWND window, const WindowSettings &window_settings ) :
		handle( window, DestroyWindow ),
		icon( nullptr, DestroyIcon ),
		context( window, window_settings )
	{
		std::memset( keyboard_buttons, BUTTON_RELEASED, sizeof( keyboard_buttons ) );
		std::memset( mouse_buttons, BUTTON_RELEASED, sizeof( mouse_buttons ) );
	}

	std::unique_ptr<HWND__, BOOL ( WINAPI * )( HWND window )> handle;
	std::unique_ptr<HICON__, BOOL ( WINAPI * )( HICON icon )> icon;
	Context context;
	Monitor monitor;
	DWORD flags;
	DWORD exflags;
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

	static Internal::WindowInitializer window_initializer;

	std::wstring widetitle;
	UTF16::FromUTF8( title.begin( ), title.end( ), std::back_inserter( widetitle ) );

	DWORD flags = WS_CLIPSIBLINGS | WS_CLIPCHILDREN, exflags = WS_EX_APPWINDOW | WS_EX_ACCEPTFILES;
	int32_t x = CW_USEDEFAULT, y = CW_USEDEFAULT, w = 0, h = 0;

	if( window_setup.monitor.IsValid( ) )
	{
		flags |= WS_POPUP;

		Vector2i pos = window_setup.monitor.GetPosition( );
		x = pos.x;
		y = pos.y;
		w = window_setup.width;
		h = window_setup.height;
	}
	else
	{
		if( window_setup.decorated )
		{
			flags |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

			if( window_setup.resizable )
			{
				flags |= WS_MAXIMIZEBOX | WS_SIZEBOX;
				exflags |= WS_EX_WINDOWEDGE;
			}
		}
		else
		{
			flags |= WS_POPUP;
		}

		Internal::GetFullWindowSize( flags, exflags, window_setup.width, window_setup.height, w, h );
	}

	HWND handle = CreateWindowEx(
		exflags,
		window_initializer.GetWindowClass( ),
		widetitle.c_str( ),
		flags,
		x, y,
		w, h,
		nullptr,
		nullptr,
		GetModuleHandle( nullptr ),
		this
	);
	if( handle == nullptr )
		return false;

#if defined MSGFLT_ALLOW

	ChangeWindowMessageFilterEx( handle, WM_DROPFILES, MSGFLT_ALLOW, nullptr );
	ChangeWindowMessageFilterEx( handle, WM_COPYDATA, MSGFLT_ALLOW, nullptr );
	ChangeWindowMessageFilterEx( handle, 0x0049, MSGFLT_ALLOW, nullptr ); // WM_COPYGLOBALDATA

#endif

	DWORD newflags = GetWindowLong( handle, GWL_STYLE );
	DWORD newexflags = GetWindowLong( handle, GWL_EXSTYLE );

	window_settings = window_setup;

	window_internal = std::make_shared<Handle>( handle, window_setup );
	window_internal->flags = newflags;
	window_internal->exflags = newexflags;
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

	std::wstring widetitle;
	UTF16::FromUTF8( title.begin( ), title.end( ), std::back_inserter( widetitle ) );
	SetWindowText( window_internal->handle.get( ), widetitle.c_str( ) );
}

void Window::SetIcon( uint32_t width, uint32_t height, const uint8_t *pixels )
{
	std::vector<uint8_t> iconPixels( width * height * 4 );
	for( std::size_t i = 0; i < iconPixels.size( ) / 4; ++i )
	{
		iconPixels[i * 4 + 0] = pixels[i * 4 + 2];
		iconPixels[i * 4 + 1] = pixels[i * 4 + 1];
		iconPixels[i * 4 + 2] = pixels[i * 4 + 0];
		iconPixels[i * 4 + 3] = pixels[i * 4 + 3];
	}

	window_internal->icon.reset( CreateIcon( GetModuleHandle( nullptr ), width, height, 1, 32, nullptr, &iconPixels[0] ) );
	if( window_internal->icon )
	{
		SendMessage( window_internal->handle.get( ), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>( window_internal->icon.get( ) ) );
		SendMessage( window_internal->handle.get( ), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>( window_internal->icon.get( ) ) );
	}
}

Vector2i Window::GetSize( ) const
{
	Vector2i size;
	if( !IsValid( ) )
		return size;

	RECT area;
	if( GetClientRect( window_internal->handle.get( ), &area ) == FALSE )
		return size;

	size.x = area.right;
	size.y = area.bottom;
	return size;
}

void Window::SetSize( const Vector2i &size )
{
	if( !IsValid( ) )
		return;

	int32_t w = 0, h = 0;
	Internal::GetFullWindowSize( window_internal->flags, window_internal->exflags, size.x, size.y, w, h );
	SetWindowPos( window_internal->handle.get( ), HWND_TOP, 0, 0, w, h, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER );
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

	POINT wpos = { 0, 0 };
	if( ClientToScreen( window_internal->handle.get( ), &wpos ) == FALSE )
		return pos;

	pos.x = wpos.x;
	pos.y = wpos.y;
	return pos;
}

void Window::SetPosition( const Vector2i &pos )
{
	if( !IsValid( ) )
		return;

	RECT rect = { pos.x, pos.y, pos.x, pos.y };
	if( AdjustWindowRectEx( &rect, window_internal->flags, FALSE, window_internal->exflags ) == FALSE )
		return;

	SetWindowPos( window_internal->handle.get( ), nullptr, rect.left, rect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE );
}

Vector2i Window::GetCursorPos( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	POINT point;
	if( ::GetCursorPos( &point ) == FALSE )
		return pos;

	if( ScreenToClient( window_internal->handle.get( ), &point ) == FALSE )
		return pos;

	pos.x = point.x;
	pos.y = point.y;
	return pos;
}

void Window::SetCursorPos( const Vector2i &pos )
{
	if( !IsValid( ) )
		return;

	POINT wpos = { pos.x, pos.y };
	if( ClientToScreen( window_internal->handle.get( ), &wpos ) == FALSE )
		return;

	::SetCursorPos( wpos.x, wpos.y );
}

std::string Window::GetClipboardString( ) const
{
	std::string clipboard;
	if( !IsValid( ) )
		return clipboard;

	if( IsClipboardFormatAvailable( CF_UNICODETEXT ) == FALSE )
		return clipboard;

	if( OpenClipboard( window_internal->handle.get( ) ) == FALSE )
		return clipboard;

	HANDLE string_handle = GetClipboardData( CF_UNICODETEXT );
	if( string_handle == nullptr )
	{
		CloseClipboard( );
		return clipboard;
	}

	wchar_t *widechar = reinterpret_cast<wchar_t *>( GlobalLock( string_handle ) );

	GlobalUnlock( string_handle );

	CloseClipboard( );

	if( widechar == nullptr )
		return clipboard;

	UTF8::FromUTF16( widechar, widechar + wcslen( widechar ), std::back_inserter( clipboard ) );
	return clipboard;
}

void Window::SetClipboardString( const std::string &text )
{
	if( !IsValid( ) )
		return;

	std::wstring widetext;
	UTF16::FromUTF8( text.begin( ), text.end( ), std::back_inserter( widetext ) );
	size_t widesize = widetext.size( ) + 1;

	HANDLE string_handle = GlobalAlloc( GMEM_MOVEABLE, widesize );
	if( string_handle == nullptr )
		return;

	void *buffer = GlobalLock( string_handle );
	if( buffer == nullptr )
		return;

	std::memcpy( buffer, widetext.c_str( ), widesize );

	GlobalUnlock( string_handle );

	if( OpenClipboard( window_internal->handle.get( ) ) == FALSE )
	{
		GlobalFree( string_handle );
		return;
	}

	EmptyClipboard( );

	SetClipboardData( CF_UNICODETEXT, string_handle );

	CloseClipboard( );
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

	return GetForegroundWindow( ) == window_internal->handle.get( );
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

	return IsIconic( window_internal->handle.get( ) ) == TRUE;
}

void Window::SetIconified( bool iconified )
{
	if( !IsValid( ) )
		return;

	ShowWindow( window_internal->handle.get( ), iconified ? SW_MINIMIZE : SW_RESTORE );
}

bool Window::IsVisible( ) const
{
	if( !IsValid( ) )
		return false;

	return IsWindowVisible( window_internal->handle.get( ) ) == TRUE;
}

void Window::SetVisible( bool visible )
{
	if( !IsValid( ) )
		return;

	ShowWindow( window_internal->handle.get( ), visible ? SW_SHOWNORMAL : SW_HIDE );
}

void Window::SwapBuffers( )
{
	if( !IsValid( ) )
		return;

	::SwapBuffers( window_internal->context.device_context );
}

void Window::SwapInterval( int32_t refreshes )
{
	if( !IsValid( ) || wglSwapIntervalEXT == nullptr )
		return;

	wglSwapIntervalEXT( refreshes );
}

void Window::PollEvents( )
{
	MSG msg;
	while( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) == TRUE )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void Window::WaitEvents( )
{
	WaitMessage( );
	PollEvents( );
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