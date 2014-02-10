#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Vector2
{
	Vector2( );
	Vector2( T x, T y );
	template<typename OT> explicit Vector2( const Vector2<OT> &vec );

	void Negate( );

	T LengthSquare( );
	float Length( );

	T DistanceSquare( const Vector2<T> &vec );
	float Distance( const Vector2<T> &vec );

	void Normalize( );
	Vector2<T> GetNormalized( );

	T &operator[]( size_t i );
	const T &operator[]( size_t i ) const;

	Vector2<T> operator-( );
	Vector2<T> &operator+=( const Vector2<T> &right );
	Vector2<T> &operator-=( const Vector2<T> &right );
	Vector2<T> operator+( const Vector2<T> &right );
	Vector2<T> operator-( const Vector2<T> &right );
	Vector2<T> operator*( T right );
	Vector2<T> &operator*=( T right );
	Vector2<T> operator/( T right );
	Vector2<T> &operator/=( T right );
	bool operator==( const Vector2<T> &right );
	bool operator!=( const Vector2<T> &right );

	T x;
	T y;
};

template<typename T>
Vector2<T> operator*( T left, const Vector2<T> &right );

#include <MultiLibrary/Common/Vector2.inl>

typedef Vector2<int32_t> Vector2i;
typedef Vector2<uint32_t> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

} // namespace MultiLibrary