#include <math.h>

template<typename T>
Vector3<T>::Vector3( ) :
	x( static_cast<T>( 0 ) ),
	y( static_cast<T>( 0 ) ),
	z( static_cast<T>( 0 ) )
{ }

template<typename T>
Vector3<T>::Vector3( T inx, T iny, T inz ) :
	x( inx ),
	y( iny ),
	z( inz )
{ }

template<typename T> template<typename OT>
Vector3<T>::Vector3( const Vector3<OT> &vec ) :
	x( static_cast<T>( vec.x ) ),
	y( static_cast<T>( vec.y ) ),
	z( static_cast<T>( vec.z ) )
{ }

template<typename T>
void Vector3<T>::Negate( )
{
	x = -x;
	y = -y;
	z = -z;
}

template<typename T>
Vector3<T> Vector3<T>::CrossProduct( const Vector3<T> &vec )
{
	return Vector3<T>( y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x );
}

template<typename T>
T Vector3<T>::DotProduct( const Vector3<T> &vec )
{
	return x * vec.x + y * vec.y + z * vec.z;
}

template<typename T>
T Vector3<T>::LengthSquare( )
{
	return x * x + y * y + z * z;
}

template<typename T>
float Vector3<T>::Length( )
{
	return sqrt( static_cast<float>( LengthSquare( ) ) );
}

template<typename T>
T Vector3<T>::DistanceSquare( const Vector3<T> &vec )
{
	return Vector3<T>( x - vec.x, y - vec.y, z - vec.z ).LengthSquare( );
}

template<typename T>
float Vector3<T>::Distance( const Vector3<T> &vec )
{
	return sqrt( static_cast<float>( DistanceSquare( vec ) ) );
}

template<typename T>
void Vector3<T>::Normalize( )
{
	float len = Length( );
	x = static_cast<T>( x / len );
	y = static_cast<T>( y / len );
	z = static_cast<T>( z / len );
}

template<typename T>
Vector3<T> Vector3<T>::GetNormalized( )
{
	Vector3<T> temp( x, y, z );
	temp.Normalize( );
	return temp;
}

template<typename T>
T &Vector3<T>::operator[]( size_t i )
{
	if( i >= 3 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
const T &Vector3<T>::operator[]( size_t i ) const
{
	if( i >= 3 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
Vector3<T> Vector3<T>::operator-( )
{
	return Vector3<T>( -x, -y, -z );
}

template<typename T>
Vector3<T> &Vector3<T>::operator+=( const Vector3<T> &right )
{
	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}

template<typename T>
Vector3<T> &Vector3<T>::operator-=( const Vector3<T> &right )
{
	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator+( const Vector3<T> &right )
{
	return Vector3<T>( x + right.x, y + right.y, z + right.z );
}

template<typename T>
Vector3<T> Vector3<T>::operator-( const Vector3<T> &right )
{
	return Vector3<T>( x - right.x, y - right.y, z - right.z );
}

template<typename T>
Vector3<T> Vector3<T>::operator*( T right )
{
	return Vector3<T>( x * right, y * right, z * right );
}

template<typename T>
Vector3<T> operator*( T left, const Vector3<T> &right )
{
	return Vector3<T>( right.x * left, right.y * left, right.z * left );
}

template<typename T>
Vector3<T> &Vector3<T>::operator*=( T right )
{
	x *= right;
	y *= right;
	z *= right;
	return *this;
}

template<typename T>
Vector3<T> Vector3<T>::operator/( T right )
{
	return Vector3<T>( x / right, y / right, z / right );
}

template<typename T>
Vector3<T> &Vector3<T>::operator/=( T right )
{
	x /= right;
	y /= right;
	z /= right;
	return *this;
}

template<typename T>
bool Vector3<T>::operator==( const Vector3<T> &right )
{
	return x == right.x && y == right.y && z == right.z;
}

template<typename T>
bool Vector3<T>::operator!=( const Vector3<T> &right )
{
	return x != right.x || y != right.y || z != right.z;
}