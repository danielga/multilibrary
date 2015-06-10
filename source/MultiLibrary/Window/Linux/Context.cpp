/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015, Daniel Almeida
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

#include <MultiLibrary/Window/Linux/Context.hpp>
#include <MultiLibrary/Window/VideoMode.hpp>
#include <MultiLibrary/Window/Window.hpp>
#include <stdexcept>

namespace MultiLibrary
{

namespace Internal
{

Context *current_context = nullptr;

}

Context::Context( Display *display, GLXDrawable window, const WindowSettings &window_setup ) :
	display_handle( display ),
	window_handle( window ),
	visual_info( nullptr ),
	context_handle( nullptr )
{
	const int pixel_attributes[] = {
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, GL_TRUE,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_RENDERABLE, GL_TRUE,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_RED_SIZE, window_setup.red_bits,
		GLX_GREEN_SIZE, window_setup.green_bits,
		GLX_BLUE_SIZE, window_setup.blue_bits,
		GLX_ALPHA_SIZE, window_setup.alpha_bits,
		GLX_DEPTH_SIZE, window_setup.depth_bits,
		GLX_STENCIL_SIZE, window_setup.stencil_bits,
		GLX_SAMPLES, window_setup.samples,
		0
	};
	int elems = 0;
	window_setup.monitor; // use for 2nd parameter of glXChooseFBConfig
	GLXFBConfig *configs = glXChooseFBConfig( display_handle, 0, pixel_attributes, &elems );
	if( configs == nullptr || elems <= 0 )
		throw std::runtime_error( "failed to choose context config" );

	if( ( visual_info = glXGetVisualFromFBConfig( display_handle, configs[0] ) ) == nullptr )
		throw std::runtime_error( "failed to get visual info from context config" );

	if( ( context_handle = glXCreateNewContext( display_handle, configs[0], GLX_RGBA_TYPE, nullptr, false ) ) == nullptr )
		throw std::runtime_error( "failed to create context" );
}

Context::~Context( )
{
	SetActive( false );

	if( context_handle != nullptr )
		glXDestroyContext( display_handle, context_handle );

	if( visual_info != nullptr )
		XFree( visual_info );
}

bool Context::SetActive( bool active )
{
	if( active && !IsActive( ) && glXMakeContextCurrent( display_handle, window_handle, window_handle, context_handle ) == GL_TRUE )
	{
		Internal::current_context = this;
		return true;
	}
	else if( !active && IsActive( ) && glXMakeContextCurrent( display_handle, None, None, nullptr ) == GL_TRUE )
	{
		Internal::current_context = nullptr;
		return true;
	}

	return false;
}

bool Context::IsActive( ) const
{
	return Internal::current_context == this && glXGetCurrentContext( ) == context_handle;
}

} // namespace MultiLibrary