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

#include <MultiLibrary/Window/Export.hpp>
#include <MultiLibrary/Window/WindowSettings.hpp>
#include <MultiLibrary/Window/Keyboard.hpp>
#include <MultiLibrary/Window/Mouse.hpp>
#include <MultiLibrary/Common/Vector2.hpp>
#include <memory>
#include <string>

namespace MultiLibrary
{

enum CursorMode
{
	CURSOR_MODE_DISABLED = -1,
	CURSOR_MODE_HIDDEN,
	CURSOR_MODE_NORMAL
};

class MULTILIBRARY_WINDOW_API Window
{
public:
	Window( );
	Window( const std::string &title, const WindowSettings &window_setup = WindowSettings( ) );
	virtual ~Window( );

	bool IsValid( ) const;

	bool Create( const std::string &title, const WindowSettings &window_setup = WindowSettings( ) );
	void Close( );

	bool ShouldClose( ) const;
	void SetShouldClose( bool close );

	bool IsActive( ) const;
	void SetActive( bool active = true );

	void SetTitle( const std::string &title );

	void SetIcon( uint32_t width, uint32_t height, const uint8_t *pixels );

	Vector2i GetSize( ) const;
	void SetSize( const Vector2i &size );

	Vector2i GetFramebufferSize( ) const;

	Vector2i GetPosition( ) const;
	void SetPosition( const Vector2i &pos );

	Vector2i GetCursorPos( ) const;
	void SetCursorPos( const Vector2i &pos );

	std::string GetClipboardString( ) const;
	void SetClipboardString( const std::string &text );

	CursorMode GetCursorMode( ) const;
	void SetCursorMode( CursorMode mode );

	bool GetStickyKeysActive( ) const;
	void SetStickyKeysActive( bool active = true );

	bool GetStickyMouseButtonsActive( ) const;
	void SetStickyMouseButtonsActive( bool active = true );

	bool IsKeyPressed( KeyCode code ) const;
	bool IsMouseButtonPressed( MouseButton code ) const;

	Monitor GetFullscreenMonitor( ) const;

	bool IsFocused( ) const;
	bool IsResizable( ) const;
	bool IsDecorated( ) const;

	bool IsIconified( ) const;
	void SetIconified( bool iconified = false );

	bool IsVisible( ) const;
	void SetVisible( bool visible = true );

	void SwapBuffers( );
	void SwapInterval( int32_t refreshes );

	static void PollEvents( );
	static void WaitEvents( );

protected:
	virtual void OnClose( );
	virtual void OnRefresh( );
	virtual void OnResize( const Vector2i &size );
	virtual void OnReposition( const Vector2i &pos );
	virtual void OnFocus( bool focused );
	virtual void OnIconify( bool iconified );
	virtual void OnFramebufferResize( const Vector2i &size );
	virtual void OnKeyPress( int32_t key, int32_t scancode, int32_t action, int32_t modifiers );
	virtual void OnCharacterInput( uint32_t codepoint );
	virtual void OnMousePress( int32_t button, int32_t action, int32_t modifiers );
	virtual void OnCursorReposition( const Vector2d &pos );
	virtual void OnCursorEnter( bool entered );
	virtual void OnScroll( const Vector2d &offset );

private:
	struct Handle;
	std::shared_ptr<Handle> window_internal;
	WindowSettings window_settings;
	bool should_close;
};

} // namespace MultiLibrary
