#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/BaseAttachable.hpp>

namespace MultiLibrary
{

enum SeekMode
{
	SEEKMODE_SET,
	SEEKMODE_CUR,
	SEEKMODE_END
};

class MULTILIBRARY_COMMON_API Stream : public BaseAttachable
{
public:
	virtual bool Seek( int64_t position, SeekMode mode = SEEKMODE_SET ) = 0;
	virtual int64_t Tell( ) const = 0;
	virtual int64_t Size( ) const = 0;
	virtual bool EndOfFile( ) const = 0;
};

} // namespace MultiLibrary