#pragma once

#include <MultiLibrary/Visual/Export.hpp>
#include <MultiLibrary/Common/Vector2.hpp>
#include <stdexcept>

namespace MultiLibrary
{

template<typename T> struct Matrix2x2
{
	enum
	{
		LineCount = 2,
		ColumnCount = 2
	};

	Matrix2x2( );

	Matrix2x2( const T &v );

	Matrix2x2(
		const T &x1, const T &y1,
		const T &x2, const T &y2
	);

	Matrix2x2(
		const Vector2<T> &column1,
		const Vector2<T> &column2
	);

	template<typename OT> explicit Matrix2x2( const Matrix2x2<OT> &mat );

	T Determinant( ) const;
	Matrix2x2<T> Inverse( ) const;
	Matrix2x2<T> Transpose( ) const;

	Vector2<T> &operator[]( size_t i );
	const Vector2<T> &operator[]( size_t i ) const;

	Matrix2x2<T> operator-( );
	Matrix2x2<T> operator+( const Matrix2x2<T> &right );
	Matrix2x2<T> &operator+=( const Matrix2x2<T> &right );
	Matrix2x2<T> operator-( const Matrix2x2<T> &right );
	Matrix2x2<T> &operator-=( const Matrix2x2<T> &right );
	Matrix2x2<T> operator*( T right );
	Matrix2x2<T> operator*( const Matrix2x2<T> &right );
	Matrix2x2<T> &operator*=( T right );
	Matrix2x2<T> &operator*=( const Matrix2x2<T> &right );
	Matrix2x2<T> operator/( T right );
	Matrix2x2<T> operator/( const Matrix2x2<T> &right );
	Matrix2x2<T> &operator/=( T right );
	Matrix2x2<T> &operator/=( const Matrix2x2<T> &right );
	bool operator==( const Matrix2x2<T> &right );
	bool operator!=( const Matrix2x2<T> &right );

	Vector2<T> columns[ColumnCount];
};

template<typename T>
Matrix2x2<T> operator*( T left, const Matrix2x2<T> &right );

#include <MultiLibrary/Visual/Matrix2x2.inl>

typedef Matrix2x2<float> Matrix2x2f;

} // namespace MultiLibrary