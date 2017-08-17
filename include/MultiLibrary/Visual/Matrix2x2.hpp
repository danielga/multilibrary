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
