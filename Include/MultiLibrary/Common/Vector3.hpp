#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Vector3
{
	Vector3( );
	Vector3( T x, T y, T z );
	template<typename OT> explicit Vector3( const Vector3<OT> &vec );

	void Negate( );

	Vector3<T> CrossProduct( const Vector3<T> &vec );
	T DotProduct( const Vector3<T> &vec );

	T LengthSquare( );
	float Length( );

	T DistanceSquare( const Vector3<T> &vec );
	float Distance( const Vector3<T> &vec );

	void Normalize( );
	Vector3<T> GetNormalized( );

	T &operator[]( size_t i );
	const T &operator[]( size_t i ) const;

	Vector3<T> operator-( );
	Vector3<T> &operator+=( const Vector3<T> &right );
	Vector3<T> &operator-=( const Vector3<T> &right );
	Vector3<T> operator+( const Vector3<T> &right );
	Vector3<T> operator-( const Vector3<T> &right );
	Vector3<T> operator*( T right );
	Vector3<T> &operator*=( T right );
	Vector3<T> operator/( T right );
	Vector3<T> &operator/=( T right );
	bool operator==( const Vector3<T> &right );
	bool operator!=( const Vector3<T> &right );

	T x;
	T y;
	T z;
};

template<typename T>
Vector3<T> operator*( T left, const Vector3<T> &right );

#include <MultiLibrary/Common/Vector3.inl>

typedef Vector3<int32_t> Vector3i;
typedef Vector3<uint32_t> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

} // namespace MultiLibrary