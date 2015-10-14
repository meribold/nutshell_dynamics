/* Copyright 2012 Lukas Waymann

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

#ifndef BODY_HPP_SEEN
#define BODY_HPP_SEEN

#include <array>
#include <cstddef> // std::size_t
#include <tuple>

#include "threeVector.hpp"

namespace nut
{
	// CRTP base class; does not handle collision or transformations:
	// solely implements functionality for collison detection
	//template <typename Derived>
	class Body
	{
		public:

		// an array of triples of indices to a ThreeVector<float>[] (i.e. triangles) and
		// that arrays length (the number of triangles); usually shared between several
		// objects
		typedef std::tuple<unsigned(*)[3], std::size_t> Pool;

		protected:

		Body() = delete; // Redundant while a user-defined ctor exists, but let's be explicit.
		Body(const Body&) = delete;
		Body(Body&&) = default;
		Body(ThreeVector<float> vertices[], ThreeVector<float> surfaceNormals[], const Pool&);

		~Body() = default;

		Body& operator=(const Body&) = delete;
		Body& operator=(Body&&) = default;

		// set up the next frame by advancing the whole scene (i.e. all active Body objects)
		// and clearing the resulting collisions, using the collision response defined
		// by the derived class
		static void advanceState(float timeInterval = 1.f);

		// Detect collisions and constructs a sequence of the respective data used to resolve
		// each collision.
		static void registerCollisions();

		// Test two objects for a collision and, if one is detected, store it and
		// return true.  Else return false.
		static bool registerCollision(const Body&, const Body&);

		const unsigned(* getFaces() const)[3] { return std::get<0>(this->pool); }

		unsigned getTriangleCount() const { return std::get<1>(this->pool); }

		// Returns nullptr if the bodies don't overlap.
		std::array<ThreeVector<float>, 2>* doesCollide(const Body&) const;

		// data unique to single Body objects; given in global cooridnates;
		// derived classes need to initialize and continuously update this data
		ThreeVector<float>* const vertices;
		ThreeVector<float>* const surfaceNormals;

		const Pool& pool; // Shared by a group of Body objects.

		private:

		std::array<ThreeVector<float>, 2>*
		doesCollide(const Body&, unsigned triangleIndex, unsigned otherIndex) const;

		static std::array<ThreeVector<float>, 2>*
		doesCollide(const Body(&)[2], const unsigned(& faceIndex)[2]);
	};
}

#endif //BODY_HPP_SEEN

// vim: tw=90 ts=2 sts=-1 sw=0 noet