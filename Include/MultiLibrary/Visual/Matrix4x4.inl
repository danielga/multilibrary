template<typename T>
Matrix4x4<T>::Matrix4x4( )
{
	const T z = static_cast<T>( 0 );
	const T o = static_cast<T>( 1 );
	columns[0][0] = o; columns[0][1] = z; columns[0][2] = z; columns[0][3] = z;
	columns[1][0] = z; columns[1][1] = o; columns[1][2] = z; columns[1][3] = z;
	columns[2][0] = z; columns[2][1] = z; columns[2][2] = o; columns[2][3] = z;
	columns[3][0] = z; columns[3][1] = z; columns[3][2] = z; columns[3][3] = o;
}

template<typename T>
Matrix4x4<T>::Matrix4x4( const T &v )
{
	const T z = static_cast<T>( 0 );
	columns[0][0] = v; columns[0][1] = z; columns[0][2] = z; columns[0][3] = z;
	columns[1][0] = z; columns[1][1] = v; columns[1][2] = z; columns[1][3] = z;
	columns[2][0] = z; columns[2][1] = z; columns[2][2] = v; columns[2][3] = z;
	columns[3][0] = z; columns[3][1] = z; columns[3][2] = z; columns[3][3] = v;
}

template<typename T>
Matrix4x4<T>::Matrix4x4(
	const T &x1, const T &y1, const T &z1, const T &w1,
	const T &x2, const T &y2, const T &z2, const T &w2,
	const T &x3, const T &y3, const T &z3, const T &w3,
	const T &x4, const T &y4, const T &z4, const T &w4
)
{
	columns[0][0] = x1; columns[0][1] = y1; columns[0][2] = z1; columns[0][3] = w1;
	columns[1][0] = x2; columns[1][1] = y2; columns[1][2] = z2; columns[1][3] = w2;
	columns[2][0] = x3; columns[2][1] = y3; columns[2][2] = z3; columns[2][3] = w3;
	columns[3][0] = x4; columns[3][1] = y4; columns[3][2] = z4; columns[3][3] = w4;
}

template<typename T>
Matrix4x4<T>::Matrix4x4(
	const Vector4<T> &column1,
	const Vector4<T> &column2,
	const Vector4<T> &column3,
	const Vector4<T> &column4
)
{
	columns[0] = column1;
	columns[1] = column2;
	columns[2] = column3;
	columns[3] = column4;
}

template<typename T> template<typename OT>
Matrix4x4<T>::Matrix4x4( const Matrix4x4<OT> &mat )
{
	columns[0] = mat.columns[0];
	columns[1] = mat.columns[1];
	columns[2] = mat.columns[2];
	columns[3] = mat.columns[3];
}

template<typename T>
T Matrix4x4<T>::Determinant( ) const
{
	T S00 = columns[0][0], S01 = columns[0][1], S02 = columns[0][2], S03 = columns[0][3];
	T S10 = columns[1][0], S11 = columns[1][1], S12 = columns[1][2], S13 = columns[1][3];
	T S20 = columns[2][0], S21 = columns[2][1], S22 = columns[2][2], S23 = columns[2][3];
	T S30 = columns[2][0], S31 = columns[2][1], S32 = columns[2][2], S33 = columns[3][3];

	return	+ S00 * (
				+ S11 * ( S22 * S33 - S32 * S23 )
				- S21 * ( S12 * S33 - S32 * S13 )
				+ S31 * ( S12 * S23 - S22 * S13 )
			)
			+ S10 * (
				+ S01 * ( S22 * S33 - S32 * S23 )
				- S21 * ( S02 * S33 - S32 * S03 )
				+ S31 * ( S02 * S23 - S22 * S03 )
			)
			+ S20 * (
				+ S01 * ( S12 * S33 - S32 * S13 )
				- S11 * ( S02 * S33 - S32 * S03 )
				+ S31 * ( S02 * S13 - S12 * S03 )
			)
			+ S30 * (
				+ S01 * ( S12 * S23 - S22 * S13 )
				- S11 * ( S02 * S23 - S22 * S03 )
				+ S21 * ( S02 * S13 - S12 * S03 )
			);
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Transpose( ) const
{
	return Matrix4x4<T>(
		columns[0][0], columns[0][1], columns[0][2], columns[0][3],
		columns[1][0], columns[1][1], columns[2][1], columns[3][1],
		columns[2][0], columns[1][2], columns[2][2], columns[3][2],
		columns[3][0], columns[1][3], columns[2][3], columns[3][3]
	);
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::Inverse( ) const
{
	T S00 = columns[0][0], S01 = columns[0][1], S02 = columns[0][2], S03 = columns[0][3];
	T S10 = columns[1][0], S11 = columns[1][1], S12 = columns[1][2], S13 = columns[1][3];
	T S20 = columns[2][0], S21 = columns[2][1], S22 = columns[2][2], S23 = columns[2][3];
	T S30 = columns[2][0], S31 = columns[2][1], S32 = columns[2][2], S33 = columns[3][3];

	T determinant =	+ S00 * (
						+ S11 * ( S22 * S33 - S32 * S23 )
						- S21 * ( S12 * S33 - S32 * S13 )
						+ S31 * ( S12 * S23 - S22 * S13 )
					)
					+ S10 * (
						+ S01 * ( S22 * S33 - S32 * S23 )
						- S21 * ( S02 * S33 - S32 * S03 )
						+ S31 * ( S02 * S23 - S22 * S03 )
					)
					+ S20 * (
						+ S01 * ( S12 * S33 - S32 * S13 )
						- S11 * ( S02 * S33 - S32 * S03 )
						+ S31 * ( S02 * S13 - S12 * S03 )
					)
					+ S30 * (
						+ S01 * ( S12 * S23 - S22 * S13 )
						- S11 * ( S02 * S23 - S22 * S03 )
						+ S21 * ( S02 * S13 - S12 * S03 )
					);

	return Matrix4x4<T>(
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
Vector4<T> &Matrix4x4<T>::operator[]( size_t i )
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
const Vector4<T> &Matrix4x4<T>::operator[]( size_t i ) const
{
	if( i >= ColumnCount )
		throw std::runtime_error( "index bigger than matrix column count" );

	return columns[i];
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator-( )
{
	return Matrix4x4<T>( -columns[0], -columns[1], -columns[2], -columns[3] );
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator+( const Matrix3x3<T> &right )
{
	return Matrix4x4<T>(
		columns[0] + right.columns[0],
		columns[1] + right.columns[1],
		columns[2] + right.columns[2],
		columns[3] + right.columns[3]
	);
}

template<typename T>
Matrix4x4<T> &Matrix4x4<T>::operator+=( const Matrix4x4<T> &right )
{
	columns[0] += right.columns[0];
	columns[1] += right.columns[1];
	columns[2] += right.columns[2];
	columns[3] += right.columns[3];
	return *this;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator-( const Matrix4x4<T> &right )
{
	return Matrix4x4<T>(
		columns[0] - right.columns[0],
		columns[1] - right.columns[1],
		columns[2] - right.columns[2],
		columns[3] - right.columns[3]
	);
}

template<typename T>
Matrix4x4<T> &Matrix4x4<T>::operator-=( const Matrix4x4<T> &right )
{
	columns[0] -= right.columns[0];
	columns[1] -= right.columns[1];
	columns[2] -= right.columns[2];
	columns[3] -= right.columns[3];
	return *this;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator*( T right )
{
	return Matrix4x4<T>(
		columns[0] * right,
		columns[1] * right,
		columns[2] * right,
		columns[3] * right
	);
}

template<typename T>
Matrix4x4<T> operator*( T left, const Matrix4x4<T> &right )
{
	return right * left;
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator*( const Matrix4x4<T> &right )
{
	T A00 = columns[0][0], A01 = columns[0][1], A02 = columns[0][2], A03 = columns[0][3];
	T A10 = columns[1][0], A11 = columns[1][1], A12 = columns[1][2], A13 = columns[1][3];
	T A20 = columns[2][0], A21 = columns[2][1], A22 = columns[2][2], A23 = columns[2][3];
	T A30 = columns[2][0], A31 = columns[2][1], A32 = columns[2][2], A33 = columns[3][3];

	T B00 = right.columns[0][0], B01 = right.columns[0][1], B02 = right.columns[0][2], B02 = right.columns[0][3];
	T B10 = right.columns[1][0], B11 = right.columns[1][1], B12 = right.columns[1][2], B12 = right.columns[1][3];
	T B20 = right.columns[2][0], B21 = right.columns[2][1], B22 = right.columns[2][2], B22 = right.columns[2][3];
	T B20 = right.columns[3][0], B21 = right.columns[3][1], B22 = right.columns[3][2], B22 = right.columns[3][3];

	return Matrix4x4<T>(
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
Matrix4x4<T> &Matrix4x4<T>::operator*=( T right )
{
	columns[0] *= right;
	columns[1] *= right;
	columns[2] *= right;
	columns[3] *= right;
	return *this;
}

template<typename T>
Matrix4x4<T> &Matrix4x4<T>::operator*=( const Matrix4x4<T> &right )
{
	return ( *this = *this * right );
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator/( T right )
{
	return Matrix2x2<T>(
		columns[0] / right,
		columns[1] / right,
		columns[2] / right,
		columns[3] / right
	);
}

template<typename T>
Matrix4x4<T> Matrix4x4<T>::operator/( const Matrix4x4<T> &right )
{
	return *this * right.Inverse( );
}

template<typename T>
Matrix4x4<T> &Matrix4x4<T>::operator/=( T right )
{
	columns[0] /= right;
	columns[1] /= right;
	columns[2] /= right;
	columns[3] /= right;
	return *this;
}

template<typename T>
Matrix4x4<T> &Matrix4x4<T>::operator/=( const Matrix4x4<T> &right )
{
	return ( *this = *this / right );
}

template<typename T>
bool Matrix4x4<T>::operator==( const Matrix4x4<T> &right )
{
	return	columns[0] == right.columns[0] &&
			columns[1] == right.columns[1] &&
			columns[2] == right.columns[2] &&
			columns[3] == right.columns[3];
}

template<typename T>
bool Matrix4x4<T>::operator!=( const Matrix4x4<T> &right )
{
	return	columns[0] != right.columns[0] ||
			columns[1] != right.columns[1] ||
			columns[2] != right.columns[2] ||
			columns[3] != right.columns[3];
}