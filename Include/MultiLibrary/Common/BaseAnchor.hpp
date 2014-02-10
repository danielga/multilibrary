#pragma once

#include <MultiLibrary/Common/Export.hpp>

namespace MultiLibrary
{

class BaseAttachable;

class MULTILIBRARY_COMMON_API BaseAnchor
{
public:
	BaseAnchor( );
	virtual ~BaseAnchor( );

	const BaseAttachable *GetAttachment( ) const;
	BaseAttachable *GetAttachment( );
	virtual void SetAttachment( BaseAttachable *base );
	virtual void ResetAttachment( );

protected:
	BaseAttachable *attachment;
};

} // namespace MultiLibrary