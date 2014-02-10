#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <MultiLibrary/Common/InputStream.hpp>
#include <MultiLibrary/Common/OutputStream.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_COMMON_API IOStream : public InputStream, public OutputStream { };

} // namespace MultiLibrary