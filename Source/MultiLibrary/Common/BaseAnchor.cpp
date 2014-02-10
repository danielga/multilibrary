#include <MultiLibrary/Common/BaseAnchor.hpp>
#include <MultiLibrary/Common/BaseAttachable.hpp>

namespace MultiLibrary
{

BaseAnchor::BaseAnchor( ) :
	attachment( NULL )
{ }

BaseAnchor::~BaseAnchor( )
{
	ResetAttachment( );
}

const BaseAttachable *BaseAnchor::GetAttachment( ) const
{
	return attachment;
}

BaseAttachable *BaseAnchor::GetAttachment( )
{
	return attachment;
}

void BaseAnchor::SetAttachment( BaseAttachable *base )
{
	ResetAttachment( );
	if( base != NULL )
	{
		attachment = base;
		attachment->Attach( this );
	}
}

void BaseAnchor::ResetAttachment( )
{
	if( attachment != NULL )
	{
		attachment->Detach( this );
		attachment = NULL;
	}
}

} // namespace MultiLibrary
