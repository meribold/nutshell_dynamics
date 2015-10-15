/* Copyright 2012 Lukas Waymann

   This file is part of Nutshell Dynamics.

   Nutshell Dynamics is free software: you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software Foundation,
   either version 3 of the License, or (at your option) any later version.

   Nutshell Dynamics is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
   PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with Nutshell
   Dynamics.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RIGIDBODY_HPP_SEEN
#define RIGIDBODY_HPP_SEEN

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef> // std::size_t
#include <memory>
#include <tuple>
#include <vector>

#include "body.hpp"
#include "modelViewMatrix.hpp"
#include "threeVector.hpp"

namespace nut
{
	class RigidBody;

	typedef std::tuple<float, RigidBody*, RigidBody*, std::array<ThreeVector<float>, 2>*>
		 CollisionContext;

	class RigidBody : Body
	{
		public:

		typedef std::tuple<ThreeVector<float>*, ThreeVector<float>*, unsigned> Pool;

		RigidBody() = delete;
		RigidBody(const RigidBody&) = delete;
		RigidBody(float mass, const float(& momentOfInertia)[3],
			const ModelViewMatrix<float>& modelViewMatrix,
			const ThreeVector<float>& velocity,
			float angularFrequency, const ThreeVector<float>& rotationAxis,
			const Body::Pool&, const RigidBody::Pool&);

		~RigidBody() = default;

		RigidBody& operator=(const RigidBody&) = delete;

		friend void advanceState(); // For constant framerates.

		friend void shiftState(float timeInterval);

		ModelViewMatrix<float>& getObjectMatrix();

		ThreeVector<float>& getVelocity() { return this->velocity; }

		protected:

		ThreeVector<float>* getVertex() const {
			return std::get<0>(this->pool);
		}

		ThreeVector<float>* getSurfaceNormal() const {
			return std::get<1>(this->pool);
		}

		unsigned getVertexCount() const {
			return std::get<2>(this->pool);
		}

		// data shared by a group of objects of nut::RigidBody
		const Pool& pool;

		ModelViewMatrix<float> modelViewMatrix;

		private:

		void move(float timeInterval = 1.f);

		static void advanceState();

		static void shiftState(float timeInterval);

		friend void refine(CollisionContext& collisionContext);

		float mass;
		float momentOfInertia[3];
		ThreeVector<float> velocity; // in world coordinates
		float angularFrequency; // i.e angular speed
		ThreeVector<float> rotationAxis; // in object coordinates

		static std::vector<RigidBody*> rigidBodies;

		void effectElasticCollision(RigidBody&, ThreeVector<float>& pointOfCollision,
			ThreeVector<float>& normal);
	};

	void advanceState();

	void shiftState(float timeInterval);

	void refine(CollisionContext& collisionContext, unsigned char iterations);

	inline ModelViewMatrix<float>& RigidBody::getObjectMatrix() {
		return this->modelViewMatrix;
	}

	extern unsigned short refineIterations;

	inline void refine(CollisionContext& collisionContext)
	{
		std::get<1>(collisionContext)->move(-.5f);
		std::get<2>(collisionContext)->move(-.5f);

		std::size_t i = 1u;

		while (i < refineIterations)
		{
			++i;

			auto newContext =
				std::get<1>(collisionContext)->doesCollide(*std::get<2>(collisionContext));

			if (newContext)
			{
				// test
				std::get<3>(collisionContext) = newContext;
					//std::get<1>(collisionContext)->doesCollide(*std::get<2>(collisionContext));
				// test
				std::get<1>(collisionContext)->move(-1.f / std::pow(2, i));
				std::get<2>(collisionContext)->move(-1.f / std::pow(2, i));
			}
			else
			{
				std::get<1>(collisionContext)->move(1.f / std::pow(2, i));
				std::get<2>(collisionContext)->move(1.f / std::pow(2, i));

				std::get<0>(collisionContext) -= 1.f / std::pow(2, i - 1);
			}
		}

		if (std::get<1>(collisionContext)->doesCollide(*std::get<2>(collisionContext)))
		{
			std::get<1>(collisionContext)->move(-1.f / std::pow(2, i));
			std::get<2>(collisionContext)->move(-1.f / std::pow(2, i));
		}
		else
		{
			std::get<0>(collisionContext) -= 1.f / std::pow(2, i);
	  }
	}

	inline void RigidBody::advanceState()
	{
		// update rigid bodies
		for (auto i : RigidBody::rigidBodies) i->move();

		std::vector<CollisionContext> collisionContexts;

		// a posteriori collision check
		for (auto i = RigidBody::rigidBodies.begin(); i != RigidBody::rigidBodies.end(); ++i)
		{
			auto j = i; ++j;
			for (; j != RigidBody::rigidBodies.end(); ++j)
			{
				if (auto partialCollisionContext = (*i)->doesCollide(**j))
				{
					collisionContexts.push_back(
						std::make_tuple(1.f, *i, *j, partialCollisionContext));

					refine(collisionContexts.back());
				}
			}
		}

		std::sort(collisionContexts.begin(), collisionContexts.end(),
			[](const CollisionContext& a, const CollisionContext& b) {
				return std::get<0>(a) < std::get<0>(b);
			}
		);

		for (const auto& i : collisionContexts)
		{
			std::get<1>(i)->effectElasticCollision(*std::get<2>(i), (*std::get<3>(i))[0],
					(*std::get<3>(i))[1]);

			std::get<1>(i)->move(std::get<0>(i));
			std::get<2>(i)->move(std::get<0>(i));

			// TODO: check for follow-up collisions.
		}
	}
}

#endif //RIGIDBODY_HPP_SEEN

// vim: tw=90 ts=2 sts=-1 sw=0 noet
