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

#include <MultiLibrary/Common/Export.hpp>
#include <stdexcept>
#include <cmath>

namespace MultiLibrary
{

/*!
 \brief Vector of 2 dimensions.

 \tparam T Type of the vector's coordinates.
 */
template<typename T> struct Vector2
{
	/*!
	 \brief Default constructor.

	 Sets all values to 0.
	 */
	Vector2( );

	/*!
	 \brief Constructor.

	 Sets the internal values to the corresponding parameters.

	 \param x X value.
	 \param y Y value.
	 */
	Vector2( T x, T y );

	/*!
	 \brief Constructor.

	 Copies the values from the other vector.

	 \tparam OT Type of the values of the other vector.
	 \param vec Vector to copy the values from.
	 */
	template<typename OT> explicit Vector2( const Vector2<OT> &vec );

	/*!
	 \brief Negate the values.
	 */
	void Negate( );

	/*!
	 \brief Get the vector square length.

	 \return Square of the length.
	 */
	T LengthSquare( );

	/*!
	 \brief Get the vector length.

	 This function is slower than LengthSquare because it uses the sqrt
	 function. It is also much more accurate. When possible, use
	 LengthSquare.

	 \return Vector length.

	 \sa LengthSquare
	 */
	float Length( );

	/*!
	 \brief Square of the distance to the other vector.

	 \param vec Other vector.

	 \return Square of the distance.
	 */
	T DistanceSquare( const Vector2<T> &vec );

	/*!
	 \brief Distance to the other vector.

	 This function is slower than DistanceSquare because it uses the sqrt
	 function. It is also much more accurate. When possible, use
	 DistanceSquare.

	 \param vec Other vector.

	 \return Distance.

	 \sa LengthSquare
	 */
	float Distance( const Vector2<T> &vec );

	/*!
	 \brief Normalize this vector.
	 */
	void Normalize( );

	/*!
	 \brief Get a normalized vector from this vector.

	 \return Normalized vector.
	 */
	Vector2<T> GetNormalized( );

	/*!
	 \brief Array indexer operator.

	 \param i Zero-based index.

	 \return Indexed value.
	 */
	T &operator[]( size_t i );

	/*!
	 \brief Array indexer operator.

	 \param i Zero-based index.

	 \return Indexed value.

	 \overload
	 */
	const T &operator[]( size_t i ) const;

	/*!
	 \brief Negation operator.

	 \return Result of the operation.
	 */
	Vector2<T> operator-( );

	/*!
	 \brief Addition assignment operator.

	 \param right Vector to add with.

	 \return Modified this object.
	 */
	Vector2<T> &operator+=( const Vector2<T> &right );

	/*!
	 \brief Subtraction assignment operator.

	 \param right Vector to subtract with.

	 \return Modified this object.
	 */
	Vector2<T> &operator-=( const Vector2<T> &right );

	/*!
	 \brief Addition operator.

	 \param right Vector to add with.

	 \return Result of the operation.
	 */
	Vector2<T> operator+( const Vector2<T> &right );

	/*!
	 \brief Subtraction operator.

	 \param right Vector to subtract with.

	 \return Result of the operation.
	 */
	Vector2<T> operator-( const Vector2<T> &right );

	/*!
	 \brief Multiplication operator.

	 \param right Vector to multiply with.

	 \return Result of the operation.
	 */
	Vector2<T> operator*( T right );

	/*!
	 \brief Multiplication assignment operator.

	 \param right Vector to multiply with.

	 \return Modified this object.
	 */
	Vector2<T> &operator*=( T right );

	/*!
	 \brief Division operator.

	 \param right Vector to divide with.

	 \return Result of the operation.
	 */
	Vector2<T> operator/( T right );

	/*!
	 \brief Division assignment operator.

	 \param right Vector to divide with.

	 \return Modified this object.
	 */
	Vector2<T> &operator/=( T right );

	/*!
	 \brief Equality operator.

	 \param right Vector to compare with.

	 \return true if the parameters are considered equivalent.
	 */
	bool operator==( const Vector2<T> &right );

	/*!
	 \brief Inequality operator.

	 \param right Vector to compare with.

	 \return true if the parameters are not considered equivalent.
	 */
	bool operator!=( const Vector2<T> &right );

	/*!
	 \brief X coordinate of this vector.
	 */
	T x;

	/*!
	 \brief Y coordinate of this vector.
	 */
	T y;
};

template<typename T>

/*!
 \brief Multiplication operator.

 \param left The first value to multiply.
 \param right The second value to multiply.

 \return Result of the operation.
 */
Vector2<T> operator*( T left, const Vector2<T> &right );

#include <MultiLibrary/Common/Vector2.inl>

/*!
 \brief A 2 dimensions vector of type int32_t.
 */
typedef Vector2<int32_t> Vector2i;

/*!
 \brief A 2 dimensions vector of type uint32_t.
 */
typedef Vector2<uint32_t> Vector2u;

/*!
 \brief A 2 dimensions vector of type float.
 */
typedef Vector2<float> Vector2f;

/*!
 \brief A 2 dimensions vector of type double.
 */
typedef Vector2<double> Vector2d;

} // namespace MultiLibrary
