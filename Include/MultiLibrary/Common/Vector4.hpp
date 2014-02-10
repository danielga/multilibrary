#pragma once

#include <MultiLibrary/Common/Export.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Vector4
{
	Vector4( );
	Vector4( T x, T y, T z, T w );
	template<typename OT> explicit Vector4( const Vector4<OT> &vec );

	void Negate( );

	T LengthSquare( );
	float Length( );

	T DistanceSquare( const Vector4<T> &vec );
	float Distance( const Vector4<T> &vec );

	void Normalize( );
	Vector4<T> GetNormalized( );

	T &operator[]( size_t i );
	const T &operator[]( size_t i ) const;

	Vector4<T> operator-( );
	Vector4<T> &operator+=( const Vector4<T> &right );
	Vector4<T> &operator-=( const Vector4<T> &right );
	Vector4<T> operator+( const Vector4<T> &right );
	Vector4<T> operator-( const Vector4<T> &right );
	Vector4<T> operator*( T right );
	Vector4<T> &operator*=( T right );
	Vector4<T> operator/( T right );
	Vector4<T> &operator/=( T right );
	bool operator==( const Vector4<T> &right );
	bool operator!=( const Vector4<T> &right );

	T x;
	T y;
	T z;
	T w;
};

template<typename T>
Vector4<T> operator*( T left, const Vector4<T> &right );

#include <MultiLibrary/Common/Vector4.inl>

typedef Vector4<int32_t> Vector4i;
typedef Vector4<uint32_t> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;

} // namespace MultiLibrary