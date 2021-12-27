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
