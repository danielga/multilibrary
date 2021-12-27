/*************************************************************************
 * MultiLibrary - https://danielga.github.io/multilibrary/
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2014-2022, Daniel Almeida
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
 *************************************************************************/

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
