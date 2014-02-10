#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/NonCopyable.hpp>
#include <MultiLibrary/Common/Time.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace MultiLibrary
{

namespace Internal
{

struct Data;

} // namespace Internal

class MULTILIBRARY_COMMON_API Process : public NonCopyable
{
public:
	Process( );
	~Process( );

	bool Start( const std::string &path, const std::string &args = "" );
	bool Start( const std::string &path, const std::vector<std::string> &args );

	bool Wait( const Time &timeout );
	bool Close( );
	void Kill( );

	void CloseInput( );

	std::ostream &Input( );
	std::istream &Output( );
	std::istream &Error( );

	int32_t ExitCode( ) const;

private:
	Internal::Data *process;
	int32_t exit_code;
};

} // namespace MultiLibrary