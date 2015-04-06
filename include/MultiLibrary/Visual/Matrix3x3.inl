/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014, Daniel Almeida
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

template<typename T>
Matrix3x3<T>::Matrix3x3( )
{
	const T z = static_cast<T>( 0 );
	const T o = static_cast<T>( 1 );
	columns[0][0] = o; columns[0][1] = z; columns[0][2] = z;
	columns[1][0] = z; columns[1][1] = o; columns[1][2] = z;
	columns[2][0] = z; columns[2][1] = z; columns[2][2] = o;
}

template<typename T>
Matrix3x3<T>::Matrix3x3( const T &v )
{
	const T z = static_cast<T>( 0 );
	columns[0][0] = v; columns[0][1] = z; columns[0][2] = z;
	columns[1][0] = z; columns[1][1] = v; columns[1][2] = z;
	columns[2][0] = z; columns[2][1] = z; columns[2][2] = v;
}

template<typename T>
Matrix3x3<T>::Matrix3x3(
	const T &x1, const T &y1, const T &z1,
	const T &x2, const T &y2, const T &z2,
	const T &x3, const T &y3, const T &z3
)
{
	columns[0][0] = x1; columns[0][1] = y1; columns[0][2] = z1;
	columns[1][0] = x2; columns[1][1] = y2; columns[1][2] = z2;
	columns[2][0] = x3; columns[2][1] = y3; columns[2][2] = z3;
}

template<typename T>
Matrix3x3<T>::Matrix3x3(
	const Vector3<T> &column1,
	const Vector3<T> &column2,
	const Vector3<T> &column3
)
{
	columns[0] = column1;
	columns[1] = column2;
	columns[2] = column3;
}

template<typename T> template<typename OT>
Matrix3x3<T>::Matrix3x3( const Matrix3x3<OT> &mat )
{
	columns[0] = mat.columns[0];
	columns[1] = mat.columns[1];
	columns[2] = mat.columns[2];
}

template<typename T>
T Matrix3x3<T>::Determinant( ) const
{
	T S00 = columns[0][0], S01 = columns[0][1], S02 = columns[0][2];
	T S10 = columns[1][0], S11 = columns[1][1], S12 = columns[1][2];
	T S20 = columns[2][0], S21 = columns[2][1], S22 = columns[2][2];

	return	+ S00 * ( S11 * S22 - S21 * S12 )
			- S10 * ( S01 * S22 - S21 * S02 )
			+ S20 * ( S01 * S12 - S11 * S02 );
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::Transpose( ) const
{
	return Matrix3x3<T>(
		columns[0][0], columns[0][1], columns[0][2],
		columns[1][0], columns[1][1], columns[2][1],
		columns[2][0], columns[1][2], columns[2][2]
	);
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::Inverse( ) const
{
	T S00 = columns[0][0], S01 = columns[0][1], S02 = columns[0][2];
	T S10 = columns[1][0], S11 = columns[1][1], S12 = columns[1][2];
	T S20 = columns[2][0], S21 = columns[2][1], S22 = columns[2][2];

	T determinant =	+ S00 * ( S11 * S22 - S21 * S12 )
					- S10 * ( S01 * S22 - S21 * S02 )
					+ S20 * ( S01 * S12 - S11 * S02 );

	return Matrix3x3<T>(
		+ ( S11 * S22 - S21 * S12 ) / determinant,
		- ( S10 * S22 - S20 * S12 ) / determinant,
		+ ( S10 * S21 - S20 * S11 ) / determinant,
		- ( S01 * S22 - S21 * S02 ) / determinant,
		+ ( S00 * S22 - S20 * S02 ) / determinant,
		- ( S00 * S21 - S20 * S01 ) / determinant,
		+ ( S01 * S12 - S11 * S02 ) / determinant,
		- ( S00 * S12 - S10 * S02 ) / determinant,
		+ ( S00 * S11 - S10 * S01 ) / determinant
	);
}

template<typename T>
Vector3<T> &Matrix3x3<T>::operator[]( size_t i )
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
const Vector3<T> &Matrix3x3<T>::operator[]( size_t i ) const
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator-( )
{
	return Matrix3x3<T>( -columns[0], -columns[1], -columns[2] );
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator+( const Matrix3x3<T> &right )
{
	return Matrix3x3<T>(
		columns[0] + right.columns[0],
		columns[1] + right.columns[1],
		columns[2] + right.columns[2]
	);
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator+=( const Matrix3x3<T> &right )
{
	columns[0] += right.columns[0];
	columns[1] += right.columns[1];
	columns[2] += right.columns[2];
	return *this;
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator-( const Matrix3x3<T> &right )
{
	return Matrix3x3<T>(
		columns[0] - right.columns[0],
		columns[1] - right.columns[1],
		columns[2] - right.columns[2]
	);
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator-=( const Matrix3x3<T> &right )
{
	columns[0] -= right.columns[0];
	columns[1] -= right.columns[1];
	columns[2] -= right.columns[2];
	return *this;
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator*( T right )
{
	return Matrix3x3<T>(
		columns[0] * right,
		columns[1] * right,
		columns[2] * right
	);
}

template<typename T>
Matrix3x3<T> operator*( T left, const Matrix3x3<T> &right )
{
	return right * left;
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator*( const Matrix3x3<T> &right )
{
	T A00 = columns[0][0], A01 = columns[0][1], A02 = columns[0][2];
	T A10 = columns[1][0], A11 = columns[1][1], A12 = columns[1][2];
	T A20 = columns[2][0], A21 = columns[2][1], A22 = columns[2][2];

	T B00 = right.columns[0][0], B01 = right.columns[0][1], B02 = right.columns[0][2];
	T B10 = right.columns[1][0], B11 = right.columns[1][1], B12 = right.columns[1][2];
	T B20 = right.columns[2][0], B21 = right.columns[2][1], B22 = right.columns[2][2];

	return Matrix3x3<T>(
		A00 * B00 + A10 * B01 + A20 * B02;
		A01 * B00 + A11 * B01 + A21 * B02;
		A02 * B00 + A12 * B01 + A22 * B02;
		A00 * B10 + A10 * B11 + A20 * B12;
		A01 * B10 + A11 * B11 + A21 * B12;
		A02 * B10 + A12 * B11 + A22 * B12;
		A00 * B20 + A10 * B21 + A20 * B22;
		A01 * B20 + A11 * B21 + A21 * B22;
		A02 * B20 + A12 * B21 + A22 * B22;
	);
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator*=( T right )
{
	columns[0] *= right;
	columns[1] *= right;
	columns[2] *= right;
	return *this;
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator*=( const Matrix3x3<T> &right )
{
	return ( *this = *this * right );
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator/( T right )
{
	return Matrix2x2<T>(
		columns[0] / right,
		columns[1] / right,
		columns[2] / right
	);
}

template<typename T>
Matrix3x3<T> Matrix3x3<T>::operator/( const Matrix3x3<T> &right )
{
	return *this * right.Inverse( );
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator/=( T right )
{
	columns[0] /= right;
	columns[1] /= right;
	columns[2] /= right;
	return *this;
}

template<typename T>
Matrix3x3<T> &Matrix3x3<T>::operator/=( const Matrix3x3<T> &right )
{
	return ( *this = *this / right );
}

template<typename T>
bool Matrix3x3<T>::operator==( const Matrix3x3<T> &right )
{
	return	columns[0] == right.columns[0] &&
			columns[1] == right.columns[1] &&
			columns[2] == right.columns[2];
}

template<typename T>
bool Matrix3x3<T>::operator!=( const Matrix3x3<T> &right )
{
	return	columns[0] != right.columns[0] ||
			columns[1] != right.columns[1] ||
			columns[2] != right.columns[2];
}