#pragma once

#include <MultiLibrary/Visual/Export.hpp>
#include <MultiLibrary/Common/Vector3.hpp>

namespace MultiLibrary
{

class MULTILIBRARY_VISUAL_API Vertex
{
public:
	Vertex( const Vector3f &vertex = Vector3f( ) );

	Vertex operator+( const Vertex &right );
	Vertex &operator+=( const Vertex &right );
	Vertex operator-( const Vertex &right );
	Vertex &operator-=( const Vertex &right );
private:
	Vector3f vertex;
};

} // namespace MultiLibrary