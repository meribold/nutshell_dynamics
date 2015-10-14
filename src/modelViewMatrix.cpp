#include <cmath>

namespace nut
{
	template <typename T>
	constexpr ModelViewMatrix<T>::ModelViewMatrix() :
		entries{T{1}, T{0}, T{0}, T{0},
		        T{0}, T{1}, T{0}, T{0},
		        T{0}, T{0}, T{1}, T{0},
		        T{0}, T{0}, T{0}, T{1}} {}

	template <typename T>
	constexpr ModelViewMatrix<T>::ModelViewMatrix(const T(& entries)[16]) :
		entries{entries[0], entries[1], entries[2], entries[3],
		        entries[4], entries[5], entries[6], entries[7],
		        entries[8], entries[9], entries[10], entries[11],
		        entries[12], entries[13], entries[14], entries[15]} {}

	template< typename T>
	ModelViewMatrix< T>::operator T*()
	{
		return this->entries;
	}

	template <typename T>
	inline const T& ModelViewMatrix<T>::operator[](std::size_t i) const
	{
		return this->entries[i];
	}

	template <typename T>
	inline T& ModelViewMatrix<T>::operator[](std::size_t i)
	{
		return this->entries[i];
	}

	template< typename T>
	ThreeVector< T>
	operator*( const ModelViewMatrix< T>& lHS, const ThreeVector< T, VERTEX>& rHS)
	{
		return ThreeVector< T>( lHS[0] * rHS[0] + lHS[4] * rHS[1] + lHS[8] * rHS[2] + lHS[12],
			lHS[1] * rHS[0] + lHS[5] * rHS[1] + lHS[9] * rHS[2] + lHS[13],
			lHS[2] * rHS[0] + lHS[6] * rHS[1] + lHS[10] * rHS[2] + lHS[14]);
	}

	template< typename T>
	ThreeVector< T>
	operator*( const ModelViewMatrix< T>& lHS, const ThreeVector< T, NORMAL>& rHS)
	{
		return ThreeVector< T>( lHS[0] * rHS[0] + lHS[4] * rHS[1] + lHS[8] * rHS[2],
			lHS[1] * rHS[0] + lHS[5] * rHS[1] + lHS[9] * rHS[2],
			lHS[2] * rHS[0] + lHS[6] * rHS[1] + lHS[10] * rHS[2]);
	}

/*	template< typename T>
	ModelViewMatrix< T> ModelViewMatrix< T>::getInverse()
	{
		float swap = (*this)[1];
		(*this)[1] = (*this)[4];
		(*this)[4] = swap;
		swap = (*this)[2];
		(*this) = (*this)[8];
		(*this)[8] = swap;
		swap = (*this)[6];
		(*this)[6] = (*this)[9];
	}
*/
	template <typename T>
	ModelViewMatrix<T>& ModelViewMatrix<T>::rotate(float angle, const float axis[3])
	{
		float sinAngle = sin( angle);
		float cosAngle = cos( angle);
		float swap[] = {(*this)[0], (*this)[1], (*this)[2],
		                (*this)[4], (*this)[5], (*this)[6]};

		float column[6] = {cosAngle + axis[0] * axis[0] * (1.f - cosAngle),
		                   axis[1] * axis[0] * (1.f - cosAngle) + axis[2] * sinAngle,
		                   axis[2] * axis[0] * (1.f - cosAngle) - axis[1] * sinAngle};

		(*this)[0] = (*this)[0] * column[0] + (*this)[4] * column[1] + (*this)[8] * column[2];
		(*this)[1] = (*this)[1] * column[0] + (*this)[5] * column[1] + (*this)[9] * column[2];
		(*this)[2] = (*this)[2] * column[0] + (*this)[6] * column[1] + (*this)[10] * column[2];

		column[0] = axis[0] * axis[1] * (1.f - cosAngle) - axis[2] * sinAngle;
		column[1] = cosAngle + axis[1] * axis[1] * (1.f - cosAngle);
		column[2] = axis[2] * axis[1] * (1.f - cosAngle) + axis[0] * sinAngle;

		(*this)[4] = swap[0] * column[0] + (*this)[4] * column[1] + (*this)[8] * column[2];
		(*this)[5] = swap[1] * column[0] + (*this)[5] * column[1] + (*this)[9] * column[2];
		(*this)[6] = swap[2] * column[0] + (*this)[6] * column[1] + (*this)[10] * column[2];

		column[0] = axis[0] * axis[2] * (1.f - cosAngle) + axis[1] * sinAngle;
		column[1] = axis[1] * axis[2] * (1.f - cosAngle) - axis[0] * sinAngle;
		column[2] = cosAngle + axis[2] * axis[2] * (1.f - cosAngle);

		(*this)[8] = swap[0] * column[0] + swap[3] * column[1] + (*this)[8] * column[2];
		(*this)[9] = swap[1] * column[0] + swap[4] * column[1] + (*this)[9] * column[2];
		(*this)[10] = swap[2] * column[0] + swap[5] * column[1] + (*this)[10] * column[2];

		return *this;
	}
}
