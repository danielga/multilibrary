#include <MultiLibrary/Common/ByteBuffer.hpp>
#include <MultiLibrary/Common/Unicode.hpp>
#include <MultiLibrary/Common/Stopwatch.hpp>
#include <MultiLibrary/Common/Thread.hpp>
#include <MultiLibrary/Common/Process.hpp>

#include <MultiLibrary/Common/Vector2.hpp>
#include <MultiLibrary/Common/Vector3.hpp>
#include <MultiLibrary/Common/Vector4.hpp>

#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <MultiLibrary/Filesystem/File.hpp>
#include <MultiLibrary/Filesystem/FilePack.hpp>

#include <MultiLibrary/Media/AudioDevice.hpp>
#include <MultiLibrary/Media/SoundBuffer.hpp>
#include <MultiLibrary/Media/Sound.hpp>
#include <MultiLibrary/Media/MediaDecoder.hpp>
#include <MultiLibrary/Media/Music.hpp>

#include <MultiLibrary/Network/HTTP.hpp>
#include <MultiLibrary/Network/FTP.hpp>

#include <MultiLibrary/Window/Window.hpp>
#include <MultiLibrary/Window/Context.hpp>

#include <MultiLibrary/OpenGL.hpp>
#include <MultiLibrary/Visual/Matrix2x2.hpp>

#include <iostream>

void TestSockets( )
{
	std::cout << ML::IPAddress( "google.com", 80 ).ToString( ) << "\n";

	ML::HTTP http( "google.com", 80 );
	ML::HTTP::Response response = http.SendRequest( ML::HTTP::Request( ), 3000 );
	std::cout << response.GetStatus( ) << "\n" << response.GetBody( ) << "\n\n";

	std::string str(
		"GET / HTTP/1.1\r\n"
		"Content-Length: 0\r\n"
		"Host: google.com\r\n"
		"From: someguy\r\n"
		"User-Agent: multilibrarynetwork\r\n"
		"\r\n"
	);
	ML::ByteBuffer buffer;
	buffer << str;

	ML::SocketTCP socket;
	socket.Connect( ML::IPAddress( "google.com", 80 ) );
	socket.Send( buffer, 0 );
	buffer.Seek( 0 );
	buffer.Resize( 1000 );
	socket.Receive( buffer, 0 );
	buffer >> str;

	std::cout << str << "\n";

	if( !buffer )
		std::cout << "Not valid\n";
}

void TestStrings( )
{
	std::string str = "κόσμε";
	std::cout << str.length( ) << " " << ML::UTF8::Length( str.begin( ), str.end( ) ) << "\n";

	str = "κόσμε";

	std::wstring wstr;
	ML::UTF8::ToWideString( str.begin( ), str.end( ), std::back_inserter( wstr ) );

	std::string bstr;
	ML::UTF8::FromWideString( wstr.begin( ), wstr.end( ), std::back_inserter( bstr ) );

	std::cout << str << " " << wstr.c_str( ) << " " << bstr.c_str( ) << "\n";
}

void TestFilesystem( )
{
	ML::Filesystem fs;
	ML::File file1 = fs.Open( "D:\\Programming\\MultiLibrary\\file.pak", "wb" );
	file1.Write( "FILEPACK001", 11 );
	unsigned long long data = 42ULL;
	file1.Write( &data, sizeof( data ) );
	data = 11ULL;
	file1.Write( &data, sizeof( data ) );
	unsigned char size = 6;
	file1.Write( &size, 1 );
	file1.Write( "fatman", 6 );
	data = 0ULL;
	file1.Write( &data, sizeof( data ) );
	file1.Write( "JUNGCHOI123", 11 );
	file1.Close( );

	ML::FilePack file2 = fs.Open( "D:\\Programming\\MultiLibrary\\file.pak", "rb+" );
	ML::File file3 = file2.Open( "fatman", "r+" );
	std::string str( 11, '\0' );
	file3.Read( &str[0], 11 );
	std::cout << str << "\n";
	file3.Seek( 0 );
	file3.Write( "JUNGCHOI321BOOM", 15 );
	file3.Flush( );
	file3.Close( );
	file2.Close( );

	std::cout << ML::Filesystem::GetExecutablePath( ) << "\n";
}

void TestAudio( )
{
	ML::AudioDevice device;

	ML::Music music;
	music.Open( "some_music.mp3" );
	music.Play( );

	ML::CurrentThread::Sleep( ML::Seconds( 100 ) );
}

void *ThreadFunction( ML::Window window )
{
	ML::Context current( window );

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

	while( window.IsValid( ) )
	{
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
	}

	return NULL;
}

void TestWindow( )
{
	ML::Matrix2x2f mat( 2, 5, 3, 1 );
	mat *= 1;

	ML::Window window( ML::VideoMode( 640, 480 ), "Test Window", ML::WindowSettings( true, false, true ), ML::ContextSettings( 4 ) );
	window.SetPosition( ML::Vector2i( 50, 50 ) );

	ML::Thread thread( &ThreadFunction, window );
	thread.Launch( );

	while( window.IsValid( ) )
	{
		ML::Window::PollEvents( );

		if( window.ShouldClose( ) )
			window.Close( );
	}
}

void TestProcess( )
{
	ML::Process process;
	if( !process.Start( "child.exe" ) )
		return;

	process.Input( ) << "nope\r\n" << "nein\r\n" << "nyet\r\n" << static_cast<int32_t>( 70000 ) << "\r\n";
	process.CloseInput( );

	std::string str;
	while( !process.Wait( ML::Microseconds( 0 ) ) )
	{
		if( std::getline( process.Output( ), str ).good( ) )
			std::cout << str.substr( 0, str.size( ) - 1 ) << '\n';
		else
			process.Output( ).clear( );

		ML::CurrentThread::Sleep( ML::Milliseconds( 10 ) );
	}

	// after process exits, keep chugging at the output
	while( std::getline( process.Output( ), str ).good( ) )
		std::cout << str.substr( 0, str.size( ) - 1 ) << '\n';
}

int main( int, char *[] )
{
	//TestSockets( );
	//TestStrings( );
	//TestFilesystem( );
	//TestAudio( );
	//TestWindow( );
	TestProcess( );
	return 0;
}