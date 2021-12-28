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

#include <MultiLibrary/Window/Windows/Context.hpp>
#include <MultiLibrary/Window/VideoMode.hpp>
#include <MultiLibrary/Window/WindowSettings.hpp>
#include <MultiLibrary/Window/Window.hpp>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/wglew.h>

namespace MultiLibrary
{

namespace Internal
{

Context *current_context = nullptr;

}

Context::Context( HWND window, const WindowSettings &window_settings ) :
	window_handle( window ),
	device_context( nullptr ),
	context_handle( nullptr )
{
	if( ( device_context = GetDC( window_handle ) ) == nullptr )
		throw std::runtime_error( "unable to get device context of window" );

	int32_t format = 0;
	if( wglChoosePixelFormatARB != nullptr )
	{
		const int32_t pixel_attributes[] = {
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, window_settings.red_bits + window_settings.green_bits + window_settings.blue_bits,
			WGL_RED_BITS_ARB, window_settings.red_bits,
			WGL_GREEN_BITS_ARB, window_settings.green_bits,
			WGL_BLUE_BITS_ARB, window_settings.blue_bits,
			WGL_ALPHA_BITS_ARB, window_settings.alpha_bits,
			WGL_DEPTH_BITS_ARB, window_settings.depth_bits,
			WGL_STENCIL_BITS_ARB, window_settings.stencil_bits,
			WGL_SAMPLE_BUFFERS_ARB, window_settings.samples == 0 ? GL_FALSE : GL_TRUE,
			WGL_SAMPLES_ARB, window_settings.samples,
			0
		};

		uint32_t num_formats = 0;
		if( wglChoosePixelFormatARB( device_context, pixel_attributes, nullptr, 1, &format, &num_formats ) == GL_FALSE )
			throw std::runtime_error( "unable to get appropriate pixel format" );
	}
	else
	{
		PIXELFORMATDESCRIPTOR pfd = {
			sizeof( PIXELFORMATDESCRIPTOR ),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			static_cast<uint8_t>( window_settings.red_bits + window_settings.green_bits + window_settings.blue_bits ),
			static_cast<uint8_t>( window_settings.red_bits ), 0,
			static_cast<uint8_t>( window_settings.green_bits ), 0,
			static_cast<uint8_t>( window_settings.blue_bits ), 0,
			static_cast<uint8_t>( window_settings.alpha_bits ), 0,
			0,
			0, 0, 0, 0,
			static_cast<uint8_t>( window_settings.depth_bits ),
			static_cast<uint8_t>( window_settings.stencil_bits ),
			0,
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		if( ( format = ChoosePixelFormat( device_context, &pfd ) ) == 0 )
			throw std::runtime_error( "unable to get appropriate pixel format" );
	}

	PIXELFORMATDESCRIPTOR pixel_format;
	if( DescribePixelFormat( device_context, format, sizeof( PIXELFORMATDESCRIPTOR ), &pixel_format ) == 0 )
		throw std::runtime_error( "unable to describe pixel format" );

	if( SetPixelFormat( device_context, format, &pixel_format ) == FALSE )
		throw std::runtime_error( "unable to set pixel format" );

	if( wglCreateContextAttribsARB != nullptr )
	{
		const int32_t context_attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, window_settings.opengl_major,
			WGL_CONTEXT_MINOR_VERSION_ARB, window_settings.opengl_minor,
			0
		};

		context_handle = wglCreateContextAttribsARB( device_context, nullptr, context_attributes );
	}
	else
		context_handle = wglCreateContext( device_context );

	if( context_handle == nullptr )
		throw std::runtime_error( "unable to create OpenGL context" );
}

Context::~Context( )
{
	SetActive( false );

	if( context_handle != nullptr )
		wglDeleteContext( context_handle );

	if( device_context != nullptr )
		ReleaseDC( window_handle, device_context );
}

bool Context::SetActive( bool active )
{
	if( active && !IsActive( ) && wglMakeCurrent( device_context, context_handle ) == TRUE )
	{
		Internal::current_context = this;
		return true;
	}
	else if( !active && IsActive( ) && wglMakeCurrent( nullptr, nullptr ) == TRUE )
	{
		Internal::current_context = nullptr;
		return true;
	}

	return false;
}

bool Context::IsActive( ) const
{
	return Internal::current_context == this && wglGetCurrentContext( ) == context_handle;
}

} // namespace MultiLibrary
