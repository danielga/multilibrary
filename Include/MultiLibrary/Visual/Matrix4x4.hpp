#pragma once

#include <MultiLibrary/Visual/Export.hpp>
#include <MultiLibrary/Common/Vector4.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Matrix4x4
{
	enum
	{
		LineCount = 4,
		ColumnCount = 4
	};

	Matrix4x4( );

	Matrix4x4( const T &v );

	Matrix4x4(
		const T &x1, const T &y1, const T &z1, const T &w1,
		const T &x2, const T &y2, const T &z2, const T &w2,
		const T &x3, const T &y3, const T &z3, const T &w3,
		const T &x4, const T &y4, const T &z4, const T &w4
	);

	Matrix4x4(
		const Vector4<T> &column1,
		const Vector4<T> &column2,
		const Vector4<T> &column3,
		const Vector4<T> &column4
	);

	template<typename OT> explicit Matrix4x4( const Matrix4x4<OT> &mat );

	T Determinant( ) const;
	Matrix4x4<T> Inverse( ) const;
	Matrix4x4<T> Transpose( ) const;

	Vector4<T> &operator[]( size_t i );
	const Vector4<T> &operator[]( size_t i ) const;

	Matrix4x4<T> operator-( );
	Matrix4x4<T> operator+( const Matrix4x4<T> &right );
	Matrix4x4<T> &operator+=( const Matrix4x4<T> &right );
	Matrix4x4<T> operator-( const Matrix4x4<T> &right );
	Matrix4x4<T> &operator-=( const Matrix4x4<T> &right );
	Matrix4x4<T> operator*( T right );
	Matrix4x4<T> operator*( const Matrix4x4<T> &right );
	Matrix4x4<T> &operator*=( T right );
	Matrix4x4<T> &operator*=( const Matrix4x4<T> &right );
	Matrix4x4<T> operator/( T right );
	Matrix4x4<T> operator/( const Matrix4x4<T> &right );
	Matrix4x4<T> &operator/=( T right );
	Matrix4x4<T> &operator/=( const Matrix4x4<T> &right );
	bool operator==( const Matrix4x4<T> &right );
	bool operator!=( const Matrix4x4<T> &right );

	Vector4<T> columns[ColumnCount];
};

template<typename T>
Matrix4x4<T> operator*( T left, const Matrix4x4<T> &right );

#include <MultiLibrary/Visual/Matrix4x4.inl>

typedef Matrix4x4<float> Matrix4x4f;

} // namespace MultiLibrary