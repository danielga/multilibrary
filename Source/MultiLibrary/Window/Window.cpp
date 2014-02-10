#include <MultiLibrary/Window/Window.hpp>
#include <MultiLibrary/Window/Context.hpp>
#include <MultiLibrary/Window/GLFW.hpp>

#include <iostream>

namespace MultiLibrary
{

Window::Window( ) :
	window_internal( NULL )
{
	Internal::InitializeGLFW( );
}

Window::Window( const VideoMode &mode, const std::string &title, const WindowSettings &window_settings, const ContextSettings &context_settings ) :
	window_internal( NULL )
{
	Internal::InitializeGLFW( );
	Create( mode, title, window_settings, context_settings );
}

Window::~Window( )
{
	Close( );
}

bool Window::IsValid( ) const
{
	return window_internal != NULL;
}

bool Window::Create( const VideoMode &mode, const std::string &title, const WindowSettings &window_settings, const ContextSettings &context_settings )
{
	glfwWindowHint( GLFW_VISIBLE, window_settings.visible ? GL_TRUE : GL_FALSE );
	glfwWindowHint( GLFW_RESIZABLE, window_settings.resizable ? GL_TRUE : GL_FALSE );
	glfwWindowHint( GLFW_DECORATED, window_settings.decorated ? GL_TRUE : GL_FALSE );
	glfwWindowHint( GLFW_SAMPLES, context_settings.samples );
	glfwWindowHint( GLFW_REFRESH_RATE, mode.refresh_rate );
	glfwWindowHint( GLFW_RED_BITS, mode.red_bits );
	glfwWindowHint( GLFW_GREEN_BITS, mode.green_bits );
	glfwWindowHint( GLFW_BLUE_BITS, mode.blue_bits );
	glfwWindowHint( GLFW_ALPHA_BITS, context_settings.alpha_bits );
	glfwWindowHint( GLFW_DEPTH_BITS, context_settings.depth_bits );
	glfwWindowHint( GLFW_STENCIL_BITS, context_settings.stencil_bits );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, context_settings.major_version );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, context_settings.minor_version );

	if( ( window_internal = glfwCreateWindow( mode.width, mode.height, title.c_str( ), window_settings.monitor.GetHandle( ), NULL ) ) == NULL )
		return false;

	glfwSetWindowUserPointer( window_internal, this );
	return true;
}

void Window::Close( )
{
	if( !IsValid( ) )
		return;

	glfwDestroyWindow( window_internal );
	window_internal = NULL;
}

bool Window::ShouldClose( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwWindowShouldClose( window_internal ) == GL_TRUE;
}

void Window::SetShouldClose( bool close )
{
	if( !IsValid( ) )
		return;

	glfwSetWindowShouldClose( window_internal, close ? GL_TRUE : GL_FALSE );
}

bool Window::IsActive( ) const
{
	WindowHandle handle = glfwGetCurrentContext( );
	return handle != NULL && handle == window_internal;
}

void Window::SetActive( bool active ) const
{
	if( !IsValid( ) )
		return;

	if( active )
		glfwMakeContextCurrent( window_internal );
	else if( IsActive( ) )
		glfwMakeContextCurrent( NULL );
}

void Window::SetTitle( const std::string &title )
{
	if( !IsValid( ) )
		return;

	glfwSetWindowTitle( window_internal, title.c_str( ) );
}

Vector2i Window::GetSize( ) const
{
	Vector2i size;
	if( !IsValid( ) )
		return size;

	glfwGetWindowSize( window_internal, &size.x, &size.y );
	return size;
}

void Window::SetSize( const Vector2i &size )
{
	if( !IsValid( ) )
		return;

	glfwSetWindowSize( window_internal, size.x, size.y );
}

Vector2i Window::GetFramebufferSize( ) const
{
	Vector2i size;
	if( !IsValid( ) )
		return size;

	glfwGetFramebufferSize( window_internal, &size.x, &size.y );
	return size;
}

Vector2i Window::GetPosition( ) const
{
	Vector2i pos;
	if( !IsValid( ) )
		return pos;

	glfwGetWindowPos( window_internal, &pos.x, &pos.y );
	return pos;
}

void Window::SetPosition( const Vector2i &pos )
{
	if( !IsValid( ) )
		return;

	glfwSetWindowPos( window_internal, pos.x, pos.y );
}

Vector2d Window::GetCursorPos( ) const
{
	Vector2d pos;
	if( !IsValid( ) )
		return pos;

	glfwGetCursorPos( window_internal, &pos.x, &pos.y );
	return pos;
}

void Window::SetCursorPos( const Vector2d &pos )
{
	if( !IsValid( ) )
		return;

	glfwSetCursorPos( window_internal, pos.x, pos.y );
}

std::string Window::GetClipboardString( ) const
{
	if( !IsValid( ) )
		return std::string( );

	return glfwGetClipboardString( window_internal );
}

void Window::SetClipboardString( const std::string &text )
{
	if( !IsValid( ) )
		return;

	glfwSetClipboardString( window_internal, text.c_str( ) );
}

CursorMode Window::GetCursorMode( ) const
{
	if( !IsValid( ) )
		return static_cast<CursorMode>( -1 );

	return static_cast<CursorMode>( glfwGetInputMode( window_internal, GLFW_CURSOR ) );
}

void Window::SetCursorMode( CursorMode mode )
{
	if( !IsValid( ) )
		return;

	glfwSetInputMode( window_internal, GLFW_CURSOR, mode );
}

bool Window::GetStickyKeysActive( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetInputMode( window_internal, GLFW_STICKY_KEYS ) == GL_TRUE;
}

void Window::SetStickyKeysActive( bool active )
{
	if( !IsValid( ) )
		return;

	glfwSetInputMode( window_internal, GLFW_STICKY_KEYS, active ? GL_TRUE : GL_FALSE );
}

bool Window::GetStickyMouseButtonsActive( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetInputMode( window_internal, GLFW_STICKY_MOUSE_BUTTONS ) == GL_TRUE;
}

void Window::SetStickyMouseButtonsActive( bool active )
{
	if( !IsValid( ) )
		return;

	glfwSetInputMode( window_internal, GLFW_STICKY_MOUSE_BUTTONS, active ? GL_TRUE : GL_FALSE );
}

bool Window::IsKeyPressed( KeyCode code ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetKey( window_internal, code ) == GLFW_PRESS;
}

bool Window::IsMouseButtonPressed( MouseButton code ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetMouseButton( window_internal, code ) == GLFW_PRESS;
}

Monitor Window::GetFullscreenMonitor( ) const
{
	if( !IsValid( ) )
		return Monitor( );

	return Monitor( glfwGetWindowMonitor( window_internal ) );
}

ContextSettings Window::GetContextSettings( ) const
{
	if( !IsValid( ) )
		return ContextSettings( );

	ContextSettings settings;
	settings.samples = glfwGetWindowAttrib( window_internal, GLFW_SAMPLES );
	settings.alpha_bits = glfwGetWindowAttrib( window_internal, GLFW_ALPHA_BITS );
	settings.depth_bits = glfwGetWindowAttrib( window_internal, GLFW_DEPTH_BITS );
	settings.stencil_bits = glfwGetWindowAttrib( window_internal, GLFW_STENCIL_BITS );
	settings.major_version = glfwGetWindowAttrib( window_internal, GLFW_CONTEXT_VERSION_MAJOR );
	settings.minor_version = glfwGetWindowAttrib( window_internal, GLFW_CONTEXT_VERSION_MINOR );
	return settings;
}

VideoMode Window::GetVideoMode( ) const
{
	if( !IsValid( ) )
		return VideoMode( );

	VideoMode mode;
	glfwGetWindowSize( window_internal, &mode.width, &mode.height );
	mode.red_bits = glfwGetWindowAttrib( window_internal, GLFW_RED_BITS );
	mode.green_bits = glfwGetWindowAttrib( window_internal, GLFW_GREEN_BITS );
	mode.blue_bits = glfwGetWindowAttrib( window_internal, GLFW_BLUE_BITS );
	mode.refresh_rate = glfwGetWindowAttrib( window_internal, GLFW_REFRESH_RATE );
	return mode;
}

bool Window::IsFocused( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetWindowAttrib( window_internal, GLFW_FOCUSED ) == GL_TRUE;
}

bool Window::IsResizable( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetWindowAttrib( window_internal, GLFW_RESIZABLE ) == GL_TRUE;
}

bool Window::IsDecorated( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetWindowAttrib( window_internal, GLFW_DECORATED ) == GL_TRUE;
}

bool Window::IsIconified( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetWindowAttrib( window_internal, GLFW_ICONIFIED ) == GL_TRUE;
}

void Window::SetIconified( bool iconified )
{
	if( !IsValid( ) )
		return;

	if( iconified )
		glfwIconifyWindow( window_internal );
	else
		glfwRestoreWindow( window_internal );
}

bool Window::IsVisible( ) const
{
	if( !IsValid( ) )
		return false;

	return glfwGetWindowAttrib( window_internal, GLFW_VISIBLE ) == GL_TRUE;
}

void Window::SetVisible( bool visible )
{
	if( !IsValid( ) )
		return;

	if( visible )
		glfwShowWindow( window_internal );
	else
		glfwHideWindow( window_internal );
}

void Window::SwapBuffers( )
{
	if( !IsValid( ) )
		return;

	glfwSwapBuffers( window_internal );
}

void Window::SwapInterval( int32_t refreshes )
{
	if( !IsValid( ) )
		return;

	Context current( *this );
	glfwSwapInterval( refreshes );
}

void Window::SetCallbackActive( WindowCallback type, bool active )
{
	switch( type )
	{
	case WINDOW_CALLBACK_CLOSE:
		glfwSetWindowCloseCallback( window_internal, active ? &CloseCallback : NULL );
		break;

	case WINDOW_CALLBACK_REFRESH:
		glfwSetWindowRefreshCallback( window_internal, active ? &RefreshCallback : NULL );
		break;

	case WINDOW_CALLBACK_RESIZE:
		glfwSetWindowSizeCallback( window_internal, active ? &ResizeCallback : NULL );
		break;

	case WINDOW_CALLBACK_REPOSITION:
		glfwSetWindowPosCallback( window_internal, active ? &RepositionCallback : NULL );
		break;

	case WINDOW_CALLBACK_FOCUS:
		glfwSetWindowFocusCallback( window_internal, active ? &FocusCallback : NULL );
		break;

	case WINDOW_CALLBACK_ICONIFY:
		glfwSetWindowIconifyCallback( window_internal, active ? &IconifyCallback : NULL );
		break;

	case WINDOW_CALLBACK_FRAMEBUFFER_RESIZE:
		glfwSetFramebufferSizeCallback( window_internal, active ? &FramebufferResizeCallback : NULL );
		break;

	case WINDOW_CALLBACK_KEY:
		glfwSetKeyCallback( window_internal, active ? &KeyPressCallback : NULL );
		break;

	case WINDOW_CALLBACK_CHARACTER:
		glfwSetCharCallback( window_internal, active ? &CharacterInputCallback : NULL );
		break;

	case WINDOW_CALLBACK_MOUSE:
		glfwSetMouseButtonCallback( window_internal, active ? &MousePressCallback : NULL );
		break;

	case WINDOW_CALLBACK_CURSOR_POSITION:
		glfwSetCursorPosCallback( window_internal, active ? &CursorRepositionCallback : NULL );
		break;

	case WINDOW_CALLBACK_CURSOR_ENTER:
		glfwSetCursorEnterCallback( window_internal, active ? &CursorEnterCallback : NULL );
		break;

	case WINDOW_CALLBACK_SCROLL:
		glfwSetScrollCallback( window_internal, active ? &ScrollCallback : NULL );
		break;

	default:
		throw std::runtime_error( "tried to (de)activate unknown callback type" );
	}
}

void Window::PollEvents( )
{
	glfwPollEvents( );
}

void Window::WaitEvents( )
{
	glfwWaitEvents( );
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

void Window::CloseCallback( WindowHandle win )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnClose( );
}

void Window::RefreshCallback( WindowHandle win )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnRefresh( );
}

void Window::ResizeCallback( WindowHandle win, int w, int h )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnResize( Vector2i( w, h ) );
}

void Window::RepositionCallback( WindowHandle win, int x, int y )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnReposition( Vector2i( x, y ) );
}

void Window::FocusCallback( WindowHandle win, int focused )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnFocus( focused == GL_TRUE );
}

void Window::IconifyCallback( WindowHandle win, int iconified )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnIconify( iconified == GL_TRUE );
}

void Window::FramebufferResizeCallback( WindowHandle win, int w, int h )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnFramebufferResize( Vector2i( w, h ) );
}

void Window::KeyPressCallback( WindowHandle win, int key, int scancode, int action, int modifiers )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnKeyPress( key, scancode, action, modifiers );
}

void Window::CharacterInputCallback( WindowHandle win, unsigned int codepoint )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnCharacterInput( codepoint );
}

void Window::MousePressCallback( WindowHandle win, int button, int action, int modifiers )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnMousePress( button, action, modifiers );
}

void Window::CursorRepositionCallback( WindowHandle win, double xpos, double ypos )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnCursorReposition( Vector2d( xpos, ypos ) );
}

void Window::CursorEnterCallback( WindowHandle win, int entered )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnCursorEnter( entered == GL_TRUE );
}

void Window::ScrollCallback( WindowHandle win, double xoffset, double yoffset )
{
	Window *window = static_cast<Window *>( glfwGetWindowUserPointer( win ) );
	if( window != NULL )
		window->OnScroll( Vector2d( xoffset, yoffset ) );
}

} // namespace MultiLibrary