#include <MultiLibrary/Common/BaseAttachable.hpp>
#include <MultiLibrary/Common/BaseAnchor.hpp>

namespace MultiLibrary
{

BaseAttachable::~BaseAttachable( )
{
	for( std::vector<BaseAnchor *>::iterator it = attached_anchors.begin( ); it != attached_anchors.end( ); ++it )
		( *it )->ResetAttachment( );
}

void BaseAttachable::Attach( BaseAnchor *base ) const
{
	attached_anchors.push_back( base );
}

void BaseAttachable::Detach( BaseAnchor *base ) const
{
	for( std::vector<BaseAnchor *>::iterator it = attached_anchors.begin( ); it != attached_anchors.end( ); ++it )
		if( *it == base )
		{
			attached_anchors.erase( it );
			return;
		}
}

} // namespace MultiLibrary