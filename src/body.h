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

#ifndef BODY_H_SEEN
#define BODY_H_SEEN

#include <array>
#include <cstddef> // std::size_t
#include <tuple>

#include "threeVector.h"

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

		// concrete type for representing a collison and its context
		struct Collision {
			Body(& bodies)[2];
			ThreeVector<float> point, surfaceNormal;
		};

		Body() = delete; // redundant while user-defined ctor exists, but I like to be explicit
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

		// detects collisions and constructs a sequence of the respective data used
		// to resolve each collision
		static void registerCollisions();

		// tests two objects for a collision and, if one is detected, stores it and returns true;
		// else false is returned
		static bool registerCollision(const Body&, const Body&);

		const unsigned(* getFaces() const)[3] { return std::get<0>(this->pool); }

		unsigned getTriangleCount() const { return std::get<1>(this->pool); }

		// returns nullptr if the bodies don't overlap
		std::array<ThreeVector<float>, 2>* doesCollide(const Body&) const;

		// data unique to single Body objects; given in global cooridnates;
		// derived classes need to initialize and continuously update this data
		ThreeVector<float>* const vertices;
		ThreeVector<float>* const surfaceNormals;

		// data shared by a group of Body objects
		const Pool& pool;

		private:

		std::array<ThreeVector<float>, 2>*
		doesCollide(const Body&, unsigned triangleIndex, unsigned otherIndex) const;

		static std::array<ThreeVector<float>, 2>*
		doesCollide(const Body(&)[2], const unsigned(& faceIndex)[2]);
	};
}

#endif //BODY_H_SEEN

