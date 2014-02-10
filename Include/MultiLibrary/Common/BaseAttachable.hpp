#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <vector>

namespace MultiLibrary
{

class BaseAnchor;

class MULTILIBRARY_COMMON_API BaseAttachable
{
public:
	virtual ~BaseAttachable( );

	virtual void Attach( BaseAnchor *base ) const;
	virtual void Detach( BaseAnchor *base ) const;

protected:
	mutable std::vector<BaseAnchor *> attached_anchors;
};

} // namespace MultiLibrary