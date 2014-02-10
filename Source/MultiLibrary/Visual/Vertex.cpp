#include <MultiLibrary/Visual/Vertex.hpp>

namespace MultiLibrary
{

Vertex::Vertex( const Vector3f &vertex ) :
	vertex( vertex )
{ }

Vertex Vertex::operator+( const Vertex &right )
{
	return vertex + right.vertex;
}

Vertex &Vertex::operator+=( const Vertex &right )
{
	vertex -= right.vertex;
	return *this;
}

Vertex Vertex::operator-( const Vertex &right )
{
	return vertex - right.vertex;
}

Vertex &Vertex::operator-=( const Vertex &right )
{
	vertex -= right.vertex;
	return *this;
}

} // namespace MultiLibrary