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

#include <MultiLibrary/Common/ByteBuffer.hpp>
#include <MultiLibrary/Common/String.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <MultiLibrary/Common/Stopwatch.hpp>
#include <MultiLibrary/Common/Process.hpp>

#include <MultiLibrary/Common/Vector2.hpp>
#include <MultiLibrary/Common/Vector3.hpp>
#include <MultiLibrary/Common/Vector4.hpp>

#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>

#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/SoundBuffer.hpp>
#include <MultiLibrary/Media/Sound.hpp>
#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <MultiLibrary/Media/Music.hpp>

#include <MultiLibrary/Network/HTTP.hpp>
#include <MultiLibrary/Network/FTP.hpp>

#include <MultiLibrary/Window/Window.hpp>

#include <MultiLibrary/OpenGL.hpp>
#include <MultiLibrary/Visual/Matrix2x2.hpp>

#include <iostream>
#include <thread>
#include <iterator>
#include <string>

using namespace std::chrono_literals;

static void TestSockets( )
{
	std::cout << ML::IPAddress( "google.com", 80 ).ToString( ) << std::endl;

	ML::HTTP http( "google.com", 80 );
	ML::HTTP::Response response = http.SendRequest( ML::HTTP::Request( ), 3000 );
	std::cout << response.GetStatus( ) << std::endl << response.GetBody( ) << std::endl << std::endl;

	ML::ByteBuffer buffer;
	char data[100] = { 0 };
	buffer >> data;
	buffer <<
		"GET / HTTP/1.1\r\n"
		"Content-Length: 0\r\n"
		"Host: google.com\r\n"
		"From: someguy\r\n"
		"User-Agent: multilibrarynetwork\r\n"
		"\r\n";

	ML::SocketTCP socket;
	socket.Connect( ML::IPAddress( "google.com", 80 ) );
	socket.Send( buffer, 0 );
	buffer.Seek( 0 );
	buffer.Resize( 1000 );
	socket.Receive( buffer, 0 );

	std::string str;
	buffer >> str;

	std::cout << str << std::endl;

	if( !buffer )
		std::cout << "Not valid\n";
}

static void TestStrings( )
{
	std::string str = "κόσμε";
	std::cout << str.length( ) << " " << ML::UTF8::Length( str.begin( ), str.end( ) ) << "\n";

	str = "κόσμε";

	std::wstring wstr;
	ML::UTF8::ToWideString( str.begin( ), str.end( ), std::back_inserter( wstr ) );

	std::string bstr;
	ML::UTF8::FromWideString( wstr.begin( ), wstr.end( ), std::back_inserter( bstr ) );

	std::cout << str << " " << wstr.c_str( ) << " " << bstr.c_str( ) << "\n";

	std::string str2 = "this/is/a/filepath.xml";
	std::string wild = "this/is/*";
	if( ML::String::WildcardCompare( str2, wild ) )
		std::cout << "lol\n";
}

static void TestFilesystem( )
{
	ML::Filesystem fs;
	ML::File file1 = fs.Open( "file.pak", "wb" );
	file1.Close( );

	std::cout << ML::Filesystem::GetExecutablePath( ) << "\n";
}

static void TestAudio( )
{
	ML::AudioDevice device;

	ML::SoundBuffer buffer;
	if( !buffer.Load( "some_music.mp3" ) )
		throw std::runtime_error( "failed to load some_music.mp3" );

	ML::Sound sound;
	sound.SetBuffer( buffer );
	sound.Play( );

	std::this_thread::sleep_for( 5000ms );
}

static void ThreadFunction( std::reference_wrapper<ML::Window> win )
{
	ML::Window &window = win;
	window.SetActive( );

	// Set the color and depth clear values
	glClearDepth( 1.0f );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	// Enable Z-buffer read and write
	glEnable( GL_DEPTH_TEST );
	glDepthMask( GL_TRUE );

	// Disable lighting and texturing
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	// Configure the viewport (the same size as the window)
	glViewport( 0, 0, window.GetSize( ).x, window.GetSize( ).y );

	// Setup a perspective projection
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	GLfloat ratio = static_cast<float>( window.GetSize( ).x ) / window.GetSize( ).y;
	glFrustum( -ratio, ratio, -1.0f, 1.0f, 1.0f, 500.0f );

	// Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
	GLfloat cube[] =
	{
		// positions    // colors (r, g, b, a)
		-50, -50, -50,  0, 0, 1, 1,
		-50,  50, -50,  0, 0, 1, 1,
		-50, -50,  50,  0, 0, 1, 1,
		-50, -50,  50,  0, 0, 1, 1,
		-50,  50, -50,  0, 0, 1, 1,
		-50,  50,  50,  0, 0, 1, 1,

		 50, -50, -50,  0, 1, 0, 1,
		 50,  50, -50,  0, 1, 0, 1,
		 50, -50,  50,  0, 1, 0, 1,
		 50, -50,  50,  0, 1, 0, 1,
		 50,  50, -50,  0, 1, 0, 1,
		 50,  50,  50,  0, 1, 0, 1,

		-50, -50, -50,  1, 0, 0, 1,
		 50, -50, -50,  1, 0, 0, 1,
		-50, -50,  50,  1, 0, 0, 1,
		-50, -50,  50,  1, 0, 0, 1,
		 50, -50, -50,  1, 0, 0, 1,
		 50, -50,  50,  1, 0, 0, 1,

		-50,  50, -50,  0, 1, 1, 1,
		 50,  50, -50,  0, 1, 1, 1,
		-50,  50,  50,  0, 1, 1, 1,
		-50,  50,  50,  0, 1, 1, 1,
		 50,  50, -50,  0, 1, 1, 1,
		 50,  50,  50,  0, 1, 1, 1,

		-50, -50, -50,  1, 0, 1, 1,
		 50, -50, -50,  1, 0, 1, 1,
		-50,  50, -50,  1, 0, 1, 1,
		-50,  50, -50,  1, 0, 1, 1,
		 50, -50, -50,  1, 0, 1, 1,
		 50,  50, -50,  1, 0, 1, 1,

		-50, -50,  50,  1, 1, 0, 1,
		 50, -50,  50,  1, 1, 0, 1,
		-50,  50,  50,  1, 1, 0, 1,
		-50,  50,  50,  1, 1, 0, 1,
		 50, -50,  50,  1, 1, 0, 1,
		 50,  50,  50,  1, 1, 0, 1,
	};

	// Enable position and color vertex components
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 7 * sizeof( GLfloat ), cube );
	glColorPointer( 4, GL_FLOAT, 7 * sizeof( GLfloat ), cube + 3 );

	// Disable normal and texture coordinates vertex components
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	ML::Stopwatch clock;
	clock.Resume( );

	ML::Stopwatch frame;
	while( window.IsValid( ) && !window.ShouldClose( ) )
	{
		frame.Resume( );

		// Clear the color and depth buffers
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Apply some transformations to rotate the cube
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity( );
		glTranslatef( 0.0f, 0.0f, -200.0f );
		glRotatef( static_cast<float>( clock.GetElapsedTime( ) / 1000.0 ) * 50.0f, 1.0f, 0.0f, 0.0f );
		glRotatef( static_cast<float>( clock.GetElapsedTime( ) / 1000.0 ) * 30.0f, 0.0f, 1.0f, 0.0f );
		glRotatef( static_cast<float>( clock.GetElapsedTime( ) / 1000.0 ) * 90.0f, 0.0f, 0.0f, 1.0f );

		// Draw the cube
		glDrawArrays( GL_TRIANGLES, 0, 36 );

		window.SwapBuffers( );

		std::cout << "Frame time: " << frame.GetElapsedTime( ) / 1000.0 << '\n';
		frame.Reset( );
	}
}

static void TestWindow( )
{
	ML::WindowSettings settings;
	settings.monitor = ML::Monitor( true );
	settings.resizable = false;
	settings.width = 1920;
	settings.height = 1080;
	settings.samples = 8;
	settings.opengl_major = 2;

	ML::Window window( "Tëst wíndow", settings );
	window.SwapInterval( 60 );

	std::thread thread( &ThreadFunction, std::ref( window ) );

	while( window.IsValid( ) && !window.ShouldClose( ) )
		ML::Window::PollEvents( );

	thread.join( );
}

static void TestProcess( )
{
	ML::Process process( "Child.exe" );

	ML::Pipe &input = process.Input( );
	const std::string instr1 = "nope", instr2 = "nein", instr3 = "nyet", instr4 = "nada";
	const int32_t innum1 = 56;
	const bool inbool1 = true;
	input << instr1 << instr2 << instr3 << instr4 << innum1 << inbool1;
	process.CloseInput( );

	while( process.GetStatus( ) != ML::Process::Status::Terminated )
		std::this_thread::sleep_for( 10ms );

	ML::Pipe &output = process.Output( );
	std::string outstr1, outstr2, outstr3, outstr4;
	int32_t outnum1 = 0;
	bool outbool1 = false;
	output >> outstr1 >> outstr2 >> outstr3 >> outstr4 >> outnum1 >> outbool1;

	if( outstr1 != instr4 || outstr2 != instr3 || outstr3 != instr2 || outstr4 != instr1 || outnum1 != innum1 || outbool1 != inbool1 )
		throw std::runtime_error( "TestProcess failed" );

	std::cout << "Exit code: " << process.ExitCode( );
}

int main( int, char ** )
{
	(void)&TestSockets;
	(void)&TestStrings;
	(void)&TestFilesystem;
	(void)&TestAudio;
	(void)&TestWindow;
	(void)&TestProcess;

	TestSockets( );
	TestStrings( );
	TestFilesystem( );
	TestAudio( );
	TestWindow( );
	TestProcess( );
	return 0;
}
