/* Copyright 2011 Lukas Waymann

   This file is part of Nutshell Dynamics.

   Nutshell Dynamics is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Nutshell Dynamics is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Nutshell Dynamics. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODELVIEWMATRIX_H_SEEN
#define MODELVIEWMATRIX_H_SEEN

#include <cstddef>

#include "threeVector.h" // needed to befriend one of its functions

#undef NULL

namespace nut
{
	template <typename T> class ModelViewMatrix;

	template <typename T>
	constexpr bool operator==(const ModelViewMatrix<T>&, const ModelViewMatrix<T>&);

	template <typename T>
	bool operator!=(const ModelViewMatrix<T>&, const ModelViewMatrix<T>&);

	template <typename T>
	ModelViewMatrix<T> operator+(const ModelViewMatrix<T>&, const ModelViewMatrix<T>&);

	template <typename T>
	ModelViewMatrix<T> operator-(const ModelViewMatrix<T>&, const ModelViewMatrix<T>&);

	template <typename T>
	ThreeVector<T>	operator*(const ModelViewMatrix<T>&, const ThreeVector<T, NORMAL>&);

	template <typename T>
	ThreeVector<T>	operator*(const ModelViewMatrix<T>&, const ThreeVector<T, VERTEX>&);

	template <typename T>
	class ModelViewMatrix
	{
		public:

		constexpr ModelViewMatrix();    // initializes to the identity matrix
		constexpr ModelViewMatrix(const ModelViewMatrix&) = default;
		constexpr ModelViewMatrix(const T(& entries)[16]);
		~ModelViewMatrix() = default;

		ModelViewMatrix& operator=(const ModelViewMatrix&) = default;

		operator const T*() const;
		operator T*();

		const T& operator[](std::size_t) const;
		T& operator[](std::size_t);

		// non-member friends so the left hand side can be converted implicitly
		friend bool operator==<>(const ModelViewMatrix&, const ModelViewMatrix&);
		friend bool operator!=<>(const ModelViewMatrix&, const ModelViewMatrix&);

		// matrix addition
		friend ModelViewMatrix operator+<T>( const ModelViewMatrix&, const ModelViewMatrix&);
		ModelViewMatrix& operator+=(const ModelViewMatrix&);

		// matrix subtraction
		friend ModelViewMatrix operator-<>( const ModelViewMatrix&, const ModelViewMatrix&);
		ModelViewMatrix& operator-=(const ModelViewMatrix&);

		ModelViewMatrix operator-() const;

		// matrix multiplication
		ModelViewMatrix& operator*=(const ModelViewMatrix&);

		// vector by matrix multiplication; also friend of ThreeVector
		friend ThreeVector<T> operator*<>(const ModelViewMatrix<T>&,
			const ThreeVector<T, VERTEX>&);

		friend ThreeVector<T> operator*<>(const ModelViewMatrix<T>&,
			const ThreeVector<T, NORMAL>&);

		friend ThreeVector<T>& ThreeVector<T, VERTEX>::multiply(const ModelViewMatrix&);
		friend ThreeVector<T>& ThreeVector<T, NORMAL>::multiply(const ModelViewMatrix&);

		ModelViewMatrix& invert();

		ModelViewMatrix getInverse() const;

		ModelViewMatrix& rotate(float angle, const float axis[3]);

		private:

		T entries[16];
	};
}

#include "modelViewMatrix.cpp"

#endif //MODELVIEWMATRIX_H_SEEN

