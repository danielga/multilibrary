#include <MultiLibrary/Common/Pipe.hpp>
#include <MultiLibrary/Filesystem/Filesystem.hpp>
#include <iostream>
#include <thread>

int main( int, char ** )
{
	ML::Pipe pipe( ML::Standard::Input::Normal, ML::Standard::Output::None );
	std::string str1, str2, str3, str4;
	int32_t num = 0;
	bool boolean;
	pipe >> str1 >> str2 >> str3 >> str4 >> num >> boolean;

	ML::Filesystem system;
	ML::File file = system.Open( "lol.txt", "w" );
	file << str4 << str3 << str2 << str1 << num << boolean;
	file.Close( );

	std::cout << str4 << str3 << str2 << str1 << num << boolean;
	return 0;
}