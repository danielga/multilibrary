#include <math.h>

template<typename T>
Vector2<T>::Vector2( ) :
	x( static_cast<T>( 0 ) ),
	y( static_cast<T>( 0 ) )
{ }

template<typename T>
Vector2<T>::Vector2( T inx, T iny ) :
	x( inx ),
	y( iny )
{ }

template<typename T> template<typename OT>
Vector2<T>::Vector2( const Vector2<OT> &vec ) :
	x( static_cast<T>( vec.x ) ),
	y( static_cast<T>( vec.y ) )
{ }

template<typename T>
void Vector2<T>::Negate( )
{
	x = -x;
	y = -y;
}

template<typename T>
T Vector2<T>::LengthSquare( )
{
	return x * x + y * y;
}

template<typename T>
float Vector2<T>::Length( )
{
	return sqrt( static_cast<float>( LengthSquare( ) ) );
}

template<typename T>
T Vector2<T>::DistanceSquare( const Vector2<T> &vec )
{
	return Vector2<T>( x - vec.x, y - vec.y ).LengthSquare( );
}

template<typename T>
float Vector2<T>::Distance( const Vector2<T> &vec )
{
	return sqrt( static_cast<float>( DistanceSquare( vec ) ) );
}

template<typename T>
void Vector2<T>::Normalize( )
{
	float len = Length( );
	x = static_cast<T>( x / len );
	y = static_cast<T>( y / len );
}

template<typename T>
Vector2<T> Vector2<T>::GetNormalized( )
{
	Vector2<T> temp( x, y );
	temp.Normalize( );
	return temp;
}

template<typename T>
T &Vector2<T>::operator[]( size_t i )
{
	if( i >= 2 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
const T &Vector2<T>::operator[]( size_t i ) const
{
	if( i >= 2 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
Vector2<T> Vector2<T>::operator-( )
{
	return Vector2<T>( -x, -y );
}

template<typename T>
Vector2<T> &Vector2<T>::operator+=( const Vector2<T> &right )
{
	x += right.x;
	y += right.y;
	return *this;
}

template<typename T>
Vector2<T> &Vector2<T>::operator-=( const Vector2<T> &right )
{
	x -= right.x;
	y -= right.y;
	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator+( const Vector2<T> &right )
{
	return Vector2<T>( x + right.x, y + right.y );
}

template<typename T>
Vector2<T> Vector2<T>::operator-( const Vector2<T> &right )
{
	return Vector2<T>( x - right.x, y - right.y );
}

template<typename T>
Vector2<T> Vector2<T>::operator*( T right )
{
	return Vector2<T>( x * right, y * right );
}

template<typename T>
Vector2<T> operator*( T left, const Vector2<T> &right )
{
	return Vector2<T>( right.x * left, right.y * left );
}

template<typename T>
Vector2<T> &Vector2<T>::operator*=( T right )
{
	x *= right;
	y *= right;
	return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator/( T right )
{
	return Vector2<T>( x / right, y / right );
}

template<typename T>
Vector2<T> &Vector2<T>::operator/=( T right )
{
	x /= right;
	y /= right;
	return *this;
}

template<typename T>
bool Vector2<T>::operator==( const Vector2<T> &right )
{
	return x == right.x && y == right.y;
}

template<typename T>
bool Vector2<T>::operator!=( const Vector2<T> &right )
{
	return x != right.x || y != right.y;
}