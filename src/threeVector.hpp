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

#ifndef THREEVECTOR_H_SEEN
#define THREEVECTOR_H_SEEN

#include <iostream>

#include "interpretation.hpp" // a guarded enum

namespace nut
{
	// most of the following forward declarations are friends of ThreeVector

	template< typename T, Interpretation interpretation = VOID>
	class ThreeVector;

	template< typename T>
	class ModelViewMatrix;

	template< typename T, Interpretation interpretation>
	constexpr bool operator==( const ThreeVector< T, interpretation>&,
		const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	bool operator!=( const ThreeVector< T, interpretation>&,
		const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	std::ostream& operator<<( std::ostream&, const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation>
	operator+( const ThreeVector< T, interpretation>&, const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation>
	operator-( const ThreeVector< T, interpretation>&, const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation> operator*( T, const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation> operator*( const ThreeVector< T, interpretation>&, T);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation> operator/( const ThreeVector< T, interpretation>&, T);

	template< typename T, Interpretation interpretation>
	T operator*( const ThreeVector< T, interpretation>&,
		const ThreeVector< T, interpretation>&);

	template< typename T, Interpretation interpretation>
	ThreeVector< T, interpretation> getCrossProduct( const ThreeVector< T, interpretation>&,
		const ThreeVector< T, interpretation>&);

	template< typename T>
	ThreeVector< T>	operator*( const ModelViewMatrix< T>&, const ThreeVector< T, VERTEX>&);

	template< typename T>
	ThreeVector< T>	operator*( const ModelViewMatrix< T>&, const ThreeVector< T, NORMAL>&);

	template < typename T, Interpretation interpretation>
	class ThreeVector
	{
		public:

		constexpr ThreeVector() = default;
		explicit constexpr ThreeVector(const ThreeVector&) = default;
		explicit constexpr ThreeVector(const T(&)[3]);
		constexpr ThreeVector(const T[3]);
		constexpr ThreeVector( T x, T y, T z = 0);

		~ThreeVector() = default;

		// returns *this
		ThreeVector< T, interpretation>& operator= (const ThreeVector&) = default;

		// conversion functions
		constexpr operator const T* () const;
		operator T*();

		constexpr T operator* () const;
		T& operator*();

		constexpr T operator[] (unsigned i) const; // const could be implicit
		T& operator[] (unsigned i);

		// non-members so the left hand side can be converted implicitly
		friend bool operator==< T, interpretation>(const ThreeVector&, const ThreeVector&);
		friend bool operator!=< T, interpretation>(const ThreeVector&, const ThreeVector&);

		friend std::ostream& operator<< <T, interpretation>( std::ostream&, const ThreeVector&);

		// vector addition
		friend ThreeVector operator+< T, interpretation>(
			const ThreeVector&, const ThreeVector&);
		ThreeVector& operator+=( const ThreeVector&);

		// unary minus; member so implicit conversion of the calling object is impossible
		ThreeVector operator-() const;

		// vector subtraction
		friend ThreeVector nut::operator-< T, interpretation>(
			const ThreeVector&, const ThreeVector&);
		ThreeVector& operator-=( const ThreeVector&);

		// scalar multiplication
		friend ThreeVector nut::operator*< T, interpretation>( T, const ThreeVector&);
		friend ThreeVector nut::operator*< T, interpretation>( const ThreeVector&, T);
		ThreeVector& operator*=( T);

		// scalar division
		friend ThreeVector operator/< T, interpretation>( const ThreeVector&, T);
		ThreeVector& operator/=( T);

		// dot product
		friend T nut::operator*< T, interpretation>( const ThreeVector&, const ThreeVector&);
		ThreeVector& operator*=( const ThreeVector&);

		// cross product
		friend ThreeVector getCrossProduct< T, interpretation>( const ThreeVector&,
			const ThreeVector&);
		ThreeVector& cross( const ThreeVector&); // result will be stored in *this

		// length or magnitude or norm
		T getNorm() const;

		// unit vector of according direction
		ThreeVector getUnitVector() const;
		ThreeVector& normalize(); // result will be stored in *this

		// vector projection
		ThreeVector getProjection( const ThreeVector& direction) const;
		ThreeVector& project( const ThreeVector& direction); // result will overwrite *this

		static constexpr T getAngle( const ThreeVector&, const ThreeVector&);

		protected:

		T entries[3];
	};

	// Partial specialization; still looks for declarations with two template parameters
	// when befriending functions.
	template< typename T>
	class ThreeVector< T, VERTEX> : public ThreeVector< T>
	{
		public:

		ThreeVector() = delete;
		ThreeVector( const ThreeVector&) = delete;

		~ThreeVector() = delete;

		// as well friend of ModelViewMatrix
		friend ThreeVector< T> operator*< T>( const ModelViewMatrix< T>&,
			const ThreeVector&);

		// I consider operator*= confusing for this type of multiplication.
		ThreeVector< T>& multiply( const ModelViewMatrix< T>&); // overwrites *this

		ThreeVector< T>& multiplyByInverse( const ModelViewMatrix< T>&);
	};

	template< typename T>
	class ThreeVector< T, NORMAL> : public ThreeVector< T>
	{
		public:

		ThreeVector() = delete;
		ThreeVector( const ThreeVector&) = delete;

		~ThreeVector() = delete;

		ThreeVector& operator=( const ThreeVector&) = delete;

		friend ThreeVector< T> operator*< T>( const ModelViewMatrix< T>&,
			const ThreeVector&);

		ThreeVector< T>& multiply( const ModelViewMatrix< T>&);

		ThreeVector< T>& multiplyByInverse( const ModelViewMatrix< T>&);
	};
}

// the implementation has to be visible at the point of instantiation, so it's part
// of the header
#include "threeVector.cpp"

#endif //THREEVECTOR_H_SEEN

// vim: tw=90 ts=2 sts=-1 sw=0 noet
