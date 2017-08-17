/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2017, Daniel Almeida
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

#include <math.h>

template<typename T>
Vector4<T>::Vector4( ) :
	x( static_cast<T>( 0 ) ),
	y( static_cast<T>( 0 ) ),
	z( static_cast<T>( 0 ) ),
	w( static_cast<T>( 0 ) )
{ }

template<typename T>
Vector4<T>::Vector4( T inx, T iny, T inz, T inw ) :
	x( inx ),
	y( iny ),
	z( inz ),
	w( inw )
{ }

template<typename T> template<typename OT>
Vector4<T>::Vector4( const Vector4<OT> &vec ) :
	x( static_cast<T>( vec.x ) ),
	y( static_cast<T>( vec.y ) ),
	z( static_cast<T>( vec.z ) ),
	w( static_cast<T>( vec.w ) )
{ }

template<typename T>
void Vector4<T>::Negate( )
{
	x = -x;
	y = -y;
	z = -z;
	w = -w;
}

template<typename T>
T Vector4<T>::LengthSquare( )
{
	return x * x + y * y + z * z + w * w;
}

template<typename T>
float Vector4<T>::Length( )
{
	return sqrt( static_cast<float>( LengthSquare( ) ) );
}

template<typename T>
T Vector4<T>::DistanceSquare( const Vector4<T> &vec )
{
	return Vector4<T>( x - vec.x, y - vec.y, z - vec.z, w - vec.w ).LengthSquare( );
}

template<typename T>
float Vector4<T>::Distance( const Vector4<T> &vec )
{
	return sqrt( static_cast<float>( DistanceSquare( vec ) ) );
}

template<typename T>
void Vector4<T>::Normalize( )
{
	float len = Length( );
	x = static_cast<T>( x / len );
	y = static_cast<T>( y / len );
	z = static_cast<T>( z / len );
	w = static_cast<T>( w / len );
}

template<typename T>
Vector4<T> Vector4<T>::GetNormalized( )
{
	Vector4<T> temp( x, y, z, w );
	temp.Normalize( );
	return temp;
}

template<typename T>
T &Vector4<T>::operator[]( size_t i )
{
	if( i >= 4 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
const T &Vector4<T>::operator[]( size_t i ) const
{
	if( i >= 4 )
		throw std::runtime_error( "index bigger than vector size" );

	return ( &x )[i];
}

template<typename T>
Vector4<T> Vector4<T>::operator-( )
{
	return Vector4<T>( -x, -y );
}

template<typename T>
Vector4<T> &Vector4<T>::operator+=( const Vector4<T> &right )
{
	x += right.x;
	y += right.y;
	z += right.z;
	w += right.w;
	return *this;
}

template<typename T>
Vector4<T> &Vector4<T>::operator-=( const Vector4<T> &right )
{
	x -= right.x;
	y -= right.y;
	z -= right.z;
	w -= right.w;
	return *this;
}

template<typename T>
Vector4<T> Vector4<T>::operator+( const Vector4<T> &right )
{
	return Vector4<T>( x + right.x, y + right.y, z + right.z, w + right.w );
}

template<typename T>
Vector4<T> Vector4<T>::operator-( const Vector4<T> &right )
{
	return Vector4<T>( x - right.x, y - right.y, z - right.z, w - right.w );
}

template<typename T>
Vector4<T> Vector4<T>::operator*( T right )
{
	return Vector4<T>( x * right, y * right, z * right, w * right );
}

template<typename T>
Vector4<T> operator*( T left, const Vector4<T> &right )
{
	return Vector4<T>( right.x * left, right.y * left, right.z * left, right.w * left );
}

template<typename T>
Vector4<T> &Vector4<T>::operator*=( T right )
{
	x *= right;
	y *= right;
	z *= right;
	w *= right;
	return *this;
}

template<typename T>
Vector4<T> Vector4<T>::operator/( T right )
{
	return Vector4<T>( x / right, y / right, z / right, w / right );
}

template<typename T>
Vector4<T> &Vector4<T>::operator/=( T right )
{
	x /= right;
	y /= right;
	z /= right;
	w /= right;
	return *this;
}

template<typename T>
bool Vector4<T>::operator==( const Vector4<T> &right )
{
	return x == right.x && y == right.y && z == right.z && w == right.w;
}

template<typename T>
bool Vector4<T>::operator!=( const Vector4<T> &right )
{
	return x != right.x || y != right.y || z != right.z || w != right.w;
}
