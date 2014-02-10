#pragma once

#include <MultiLibrary/Visual/Export.hpp>
#include <MultiLibrary/Common/Vector3.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Matrix3x3
{
	enum
	{
		LineCount = 3,
		ColumnCount = 3
	};

	Matrix3x3( );

	Matrix3x3( const T &v );

	Matrix3x3(
		const T &x1, const T &y1, const T &z1,
		const T &x2, const T &y2, const T &z2,
		const T &x3, const T &y3, const T &z3
	);

	Matrix3x3(
		const Vector3<T> &column1,
		const Vector3<T> &column2,
		const Vector3<T> &column3
	);

	template<typename OT> explicit Matrix3x3( const Matrix3x3<OT> &mat );

	T Determinant( ) const;
	Matrix3x3<T> Inverse( ) const;
	Matrix3x3<T> Transpose( ) const;

	Vector3<T> &operator[]( size_t i );
	const Vector3<T> &operator[]( size_t i ) const;

	Matrix3x3<T> operator-( );
	Matrix3x3<T> operator+( const Matrix3x3<T> &right );
	Matrix3x3<T> &operator+=( const Matrix3x3<T> &right );
	Matrix3x3<T> operator-( const Matrix3x3<T> &right );
	Matrix3x3<T> &operator-=( const Matrix3x3<T> &right );
	Matrix3x3<T> operator*( T right );
	Matrix3x3<T> operator*( const Matrix3x3<T> &right );
	Matrix3x3<T> &operator*=( T right );
	Matrix3x3<T> &operator*=( const Matrix3x3<T> &right );
	Matrix3x3<T> operator/( T right );
	Matrix3x3<T> operator/( const Matrix3x3<T> &right );
	Matrix3x3<T> &operator/=( T right );
	Matrix3x3<T> &operator/=( const Matrix3x3<T> &right );
	bool operator==( const Matrix3x3<T> &right );
	bool operator!=( const Matrix3x3<T> &right );

	Vector3<T> columns[ColumnCount];
};

template<typename T>
Matrix3x3<T> operator*( T left, const Matrix3x3<T> &right );

#include <MultiLibrary/Visual/Matrix3x3.inl>

typedef Matrix3x3<float> Matrix3x3f;

} // namespace MultiLibrary