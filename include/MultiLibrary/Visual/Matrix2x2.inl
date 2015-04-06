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
Matrix2x2<T>::Matrix2x2( )
{
	const T z = static_cast<T>( 0 );
	const T o = static_cast<T>( 1 );
	columns[0][0] = o; columns[0][1] = z;
	columns[1][0] = z; columns[1][1] = o;
}

template<typename T>
Matrix2x2<T>::Matrix2x2( const T &v )
{
	const T z = static_cast<T>( 0 );
	columns[0][0] = v; columns[0][1] = z;
	columns[1][0] = z; columns[1][1] = v;
}

template<typename T>
Matrix2x2<T>::Matrix2x2(
	const T &x1, const T &y1,
	const T &x2, const T &y2
)
{
	columns[0][0] = x1; columns[0][1] = y1;
	columns[1][0] = x2; columns[1][1] = y2;
}

template<typename T>
Matrix2x2<T>::Matrix2x2(
	const Vector2<T> &column1,
	const Vector2<T> &column2
)
{
	columns[0] = column1;
	columns[1] = column2;
}

template<typename T> template<typename OT>
Matrix2x2<T>::Matrix2x2( const Matrix2x2<OT> &mat )
{
	columns[0] = mat.columns[0];
	columns[1] = mat.columns[1];
}

template<typename T>
T Matrix2x2<T>::Determinant( ) const
{
	return columns[0][0] * columns[1][1] - columns[1][0] * columns[0][1];
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::Transpose( ) const
{
	return Matrix2x2<T>(
		columns[0][0], columns[0][1],
		columns[1][0], columns[1][1]
	);
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::Inverse( ) const
{
	T S00 = columns[0][0], S01 = columns[0][1];
	T S10 = columns[1][0], S11 = columns[1][1];

	T determinant = S00 * S11 - S01 * S10;

	return Matrix2x2<T>(
		+ S11 / determinant,
		- S01 / determinant,
		- S10 / determinant,
		+ S00 / determinant
	);
}

template<typename T>
Vector2<T> &Matrix2x2<T>::operator[]( size_t i )
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
const Vector2<T> &Matrix2x2<T>::operator[]( size_t i ) const
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator-( )
{
	return Matrix2x2<T>( -columns[0], -columns[1] );
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator+( const Matrix2x2<T> &right )
{
	return Matrix2x2<T>(
		columns[0] + right.columns[0],
		columns[1] + right.columns[1]
	);
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator+=( const Matrix2x2<T> &right )
{
	columns[0] += right.columns[0];
	columns[1] += right.columns[1];
	return *this;
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator-( const Matrix2x2<T> &right )
{
	return Matrix2x2<T>(
		columns[0] - right.columns[0],
		columns[1] - right.columns[1]
	);
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator-=( const Matrix2x2<T> &right )
{
	columns[0] -= right.columns[0];
	columns[1] -= right.columns[1];
	return *this;
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator*( T right )
{
	return Matrix2x2<T>(
		columns[0] * right,
		columns[1] * right
	);
}

template<typename T>
Matrix2x2<T> operator*( T left, const Matrix2x2<T> &right )
{
	return right * left;
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator*( const Matrix2x2<T> &right )
{
	T A00 = columns[0][0], A01 = columns[0][1];
	T A10 = columns[1][0], A11 = columns[1][1];

	T B00 = right.columns[0][0], B01 = right.columns[0][1];
	T B10 = right.columns[1][0], B11 = right.columns[1][1];

	return Matrix2x2<T>(
		A00 * B00 + A10 * B01,
		A00 * B10 + A10 * B11,
		A01 * B00 + A11 * B01,
		A01 * B10 + A11 * B11
	);
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator*=( T right )
{
	columns[0] *= right;
	columns[1] *= right;
	return *this;
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator*=( const Matrix2x2<T> &right )
{
	return ( *this = *this * right );
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator/( T right )
{
	return Matrix2x2<T>(
		columns[0] / right,
		columns[1] / right
	);
}

template<typename T>
Matrix2x2<T> Matrix2x2<T>::operator/( const Matrix2x2<T> &right )
{
	return *this * right.Inverse( );
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator/=( T right )
{
	columns[0] /= right;
	columns[1] /= right;
	return *this;
}

template<typename T>
Matrix2x2<T> &Matrix2x2<T>::operator/=( const Matrix2x2<T> &right )
{
	return ( *this = *this / right );
}

template<typename T>
bool Matrix2x2<T>::operator==( const Matrix2x2<T> &right )
{
	return columns[0] == right.columns[0] && columns[1] == right.columns[1];
}

template<typename T>
bool Matrix2x2<T>::operator!=( const Matrix2x2<T> &right )
{
	return columns[0] != right.columns[0] || columns[1] != right.columns[1];
}