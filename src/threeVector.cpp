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

#include <cmath>

namespace nut
{
	template< typename T, Interpretation interpretation>
	constexpr ThreeVector< T, interpretation>::ThreeVector( const T(& entries)[3]) :
		entries{ entries[0], entries[1], entries[2]} {}

	template< typename T, Interpretation interpretation>
	constexpr ThreeVector< T, interpretation>::ThreeVector( const T entries[3]) :
		entries{ entries[0], entries[1], entries[2]} {}

	template< typename T, Interpretation interpretation>
	constexpr ThreeVector< T, interpretation>::ThreeVector( T x, T y, T z) :
		entries{ x, y, z} {}

	template< typename T, Interpretation interpretation>
	inline constexpr ThreeVector< T, interpretation>::operator const T*() const
	{
		return this->entries;
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>::operator T*()
	{
		return this->entries;
	}

	template< typename T, Interpretation interpretation>
	inline constexpr T ThreeVector< T, interpretation>::operator[]( unsigned i) const
	{
		return this->entries[i];
	}

	template< typename T, Interpretation interpretation>
	inline T& ThreeVector< T, interpretation>::operator[]( unsigned i)
	{
		return this->entries[i];
	}

	template< typename T, Interpretation interpretation>
	inline constexpr bool operator==( const ThreeVector< T, interpretation> &lHS,
		const ThreeVector< T, interpretation> &rHS)
	{
		return rHS[0] == lHS[0] && rHS[1] == lHS[1] && rHS[2] == lHS[2];
	}

	template< typename T, Interpretation interpretation>
	inline std::ostream& operator<<( std::ostream& os,
		const ThreeVector< T, interpretation>& threeVector)
	{
		return os << "{" << threeVector[0] << ", " << threeVector[1] << ", " <<
			threeVector[2] << "}";
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation> operator+(
		const ThreeVector< T, interpretation>& rHS,	const ThreeVector< T, interpretation>& lHS)
	{
		return {rHS[0] + lHS[0], rHS[1] + lHS[1], rHS[2] + lHS[2]};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>&
	ThreeVector< T, interpretation>::operator+=( const ThreeVector< T, interpretation> &rHS)
	{
		this->entries[0] += rHS.entries[0];
		this->entries[1] += rHS.entries[1];
		this->entries[2] += rHS.entries[2];

		return *this;
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation> operator-(
		const ThreeVector< T, interpretation>& rHS,	const ThreeVector< T, interpretation>& lHS)
	{
		return {rHS[0] - lHS[0], rHS[1] - lHS[1], rHS[2] - lHS[2]};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation> ThreeVector< T, interpretation>::operator-() const
	{
		return {-this->entries[0], -this->entries[1], -this->entries[2]};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>
	operator*( T lHS, const ThreeVector< T, interpretation>& rHS)
	{
		return {lHS * rHS[0], lHS * rHS[1], lHS * rHS[2]};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>
	operator*( const ThreeVector< T, interpretation>& lHS, T rHS)
	{
		return {lHS[0] * rHS, lHS[1] * rHS, lHS[2] * rHS};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>
	operator/( const ThreeVector< T, interpretation>& lHS, T rHS)
	{
		return {lHS[0] / rHS, lHS[1] / rHS, lHS[2] / rHS};
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>& ThreeVector< T, interpretation>::operator/=( T rHS)
	{
		this[0] /= rHS; this[1] /= rHS; this[2] /= rHS;
		return *this;
	}

	template< typename T, Interpretation interpretation>
	inline float operator*( const ThreeVector<T, interpretation>& lHS,
		const ThreeVector<T, interpretation>& rHS)
	{
		return lHS[0] * rHS[0] + lHS[1] * rHS[1] + lHS[2] * rHS[2];
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>
	getCrossProduct( const ThreeVector<T, interpretation>& lHS,
		const ThreeVector<T, interpretation>& rHS)
	{
		return {lHS[1] * rHS[2] - lHS[2] * rHS[1],
			lHS[2] * rHS[0] - lHS[0] * rHS[2],
			lHS[0] * rHS[1] - lHS[1] * rHS[0]};
	}

	template< typename T, Interpretation interpretation>
	inline T ThreeVector< T, interpretation>::getNorm() const
	{
		return std::sqrt( *this * *this);
	}

	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>
	ThreeVector< T, interpretation>::getUnitVector() const
	{
		return *this / this->getNorm();
	}

/*	template< typename T, Interpretation interpretation>
	inline ThreeVector< T, interpretation>& ThreeVector< T, interpretation>::normalize()
	{
		return *this /= this->getNorm();
	}*/

	template <typename T, Interpretation interpretation>
	inline constexpr T ThreeVector< T, interpretation>::getAngle(
		const ThreeVector< T, interpretation>& first,
		const ThreeVector< T, interpretation>& second)
	{
		return std::acos(first * second / std::sqrt((first * first) * (second * second)));
	}

/*	template< typename T>
	ThreeVector< T>&
	ThreeVector< T, VERTEX>::multiply( const ModelViewMatrix< T>& rHS)
	{
		return *this;
	}

	template< typename T>
	ThreeVector< T>&
	ThreeVector< T, NORMAL>::multiply( const ModelViewMatrix< T>& rHS)
	{
		return *this;
	}*/

	template< typename T>
	ThreeVector< T>& ThreeVector< T, NORMAL>::multiplyByInverse( const ModelViewMatrix< T>& rHS)
	{
		float swap[] = {(*this)[0], (*this)[1]};

		(*this)[0] = rHS[0] * (*this)[0] + rHS[1] * (*this)[1] + rHS[2] * (*this)[2];
		(*this)[1] = rHS[4] * swap[0] + rHS[5] * (*this)[1] + rHS[6] * (*this)[2];
		(*this)[2] = rHS[8] * swap[0] + rHS[9] * swap[1] + rHS[10] * (*this)[2];

		return *this;
	}
}

// vim: tw=90 ts=2 sts=-1 sw=0 noet
