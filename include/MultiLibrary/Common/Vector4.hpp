/*************************************************************************
 * MultiLibrary - danielga.bitbucket.org/multilibrary
 * A C++ library that covers multiple low level systems.
 *------------------------------------------------------------------------
 * Copyright (c) 2015, Daniel Almeida
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

#include <MultiLibrary/Common/Export.hpp>
#include <stdexcept>

namespace MultiLibrary
{

/*!
 \brief Vector of 4 dimensions.

 \tparam T Type of the vector's coordinates.
 */
template<typename T> struct Vector4
{
	/*!
	 \brief Default constructor.

	 Sets all values to 0.
	 */
	Vector4( );

	/*!
	 \brief Constructor.

	 Sets the internal values to the corresponding parameters.

	 \param x X value.
	 \param y Y value.
	 \param z Z value.
	 \param w W value.
	 */
	Vector4( T x, T y, T z, T w );

	/*!
	 \brief Constructor.

	 Copies the values from the other vector.

	 \tparam OT Type of the values of the other vector.
	 \param vec Vector to copy the values from.
	 */
	template<typename OT> explicit Vector4( const Vector4<OT> &vec );

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
	T DistanceSquare( const Vector4<T> &vec );

	/*!
	 \brief Distance to the other vector.

	 This function is slower than DistanceSquare because it uses the sqrt
	 function. It is also much more accurate. When possible, use
	 DistanceSquare.

	 \param vec Other vector.

	 \return Distance.

	 \sa LengthSquare
	 */
	float Distance( const Vector4<T> &vec );

	/*!
	 \brief Normalize this vector.
	 */
	void Normalize( );

	/*!
	 \brief Get a normalized vector from this vector.

	 \return Normalized vector.
	 */
	Vector4<T> GetNormalized( );

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
	Vector4<T> operator-( );

	/*!
	 \brief Addition assignment operator.

	 \param right Vector to add with.

	 \return Modified this object.
	 */
	Vector4<T> &operator+=( const Vector4<T> &right );

	/*!
	 \brief Subtraction assignment operator.

	 \param right Vector to subtract with.

	 \return Modified this object.
	 */
	Vector4<T> &operator-=( const Vector4<T> &right );

	/*!
	 \brief Addition operator.

	 \param right Vector to add with.

	 \return Result of the operation.
	 */
	Vector4<T> operator+( const Vector4<T> &right );

	/*!
	 \brief Subtraction operator.

	 \param right Vector to subtract with.

	 \return Result of the operation.
	 */
	Vector4<T> operator-( const Vector4<T> &right );

	/*!
	 \brief Multiplication operator.

	 \param right Vector to multiply with.

	 \return Result of the operation.
	 */
	Vector4<T> operator*( T right );

	/*!
	 \brief Multiplication assignment operator.

	 \param right Vector to multiply with.

	 \return Modified this object.
	 */
	Vector4<T> &operator*=( T right );

	/*!
	 \brief Division operator.

	 \param right Vector to divide with.

	 \return Result of the operation.
	 */
	Vector4<T> operator/( T right );

	/*!
	 \brief Division assignment operator.

	 \param right Vector to divide with.

	 \return Modified this object.
	 */
	Vector4<T> &operator/=( T right );

	/*!
	 \brief Equality operator.

	 \param right Vector to compare with.

	 \return true if the parameters are considered equivalent.
	 */
	bool operator==( const Vector4<T> &right );

	/*!
	 \brief Inequality operator.

	 \param right Vector to compare with.

	 \return true if the parameters are not considered equivalent.
	 */
	bool operator!=( const Vector4<T> &right );

	/*!
	 \brief X coordinate of this vector.
	 */
	T x;

	/*!
	 \brief Y coordinate of this vector.
	 */
	T y;

	/*!
	 \brief Z coordinate of this vector.
	 */
	T z;

	/*!
	 \brief W coordinate of this vector.
	 */
	T w;
};

/*!
 \brief Multiplication operator.

 \param left Value to multiply the vector.
 \param right Vector to multiply.

 \return Result of the operation.
 */
template<typename T>
Vector4<T> operator*( T left, const Vector4<T> &right );

#include <MultiLibrary/Common/Vector4.inl>

/*!
 \brief A 4 dimensions vector of type int32_t.
 */
typedef Vector4<int32_t> Vector4i;

/*!
 \brief A 4 dimensions vector of type uint32_t.
 */
typedef Vector4<uint32_t> Vector4u;

/*!
 \brief A 4 dimensions vector of type float.
 */
typedef Vector4<float> Vector4f;

/*!
 \brief A 4 dimensions vector of type double.
 */
typedef Vector4<double> Vector4d;

} // namespace MultiLibrary