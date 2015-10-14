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

#include <cstdlib>

#include "rigidBody.h"

namespace nut
{
	std::vector<RigidBody*> RigidBody::rigidBodies;

	RigidBody::RigidBody(float mass, const float(& momentOfInertia)[3],
		const ModelViewMatrix<float>& modelViewMatrix,
		const ThreeVector<float>& velocity,
		float angularFrequency, const ThreeVector<float>& rotationAxis,
		const Body::Pool& bodyPool,
		const RigidBody::Pool& rigidBodyPool) :
			Body{reinterpret_cast<ThreeVector<float>*>(
			     std::malloc(std::get<2>(rigidBodyPool) * sizeof(ThreeVector<float>))),
			     reinterpret_cast<ThreeVector<float>*>(
			     std::malloc(std::get<1>(bodyPool) * sizeof(ThreeVector<float>))),
			     bodyPool},
			pool(rigidBodyPool), modelViewMatrix{modelViewMatrix}, mass{mass},
			momentOfInertia{momentOfInertia[0], momentOfInertia[1], momentOfInertia[2]},
			velocity(velocity), angularFrequency(angularFrequency),
			rotationAxis(rotationAxis)
	{
		RigidBody::rigidBodies.push_back(this);
	}

	void RigidBody::move(float timeInterval)
	{
		this->modelViewMatrix[12] += this->velocity[0] * timeInterval;
		this->modelViewMatrix[13] += this->velocity[1] * timeInterval;
		this->modelViewMatrix[14] += this->velocity[2] * timeInterval;

		this->modelViewMatrix.rotate( this->angularFrequency * timeInterval,
			this->rotationAxis);

		// transforming members of body to new global coordinates
		for (unsigned i = 0; i != this->getVertexCount(); ++i)
		{
			this->vertices[i] = this->modelViewMatrix *
				static_cast<ThreeVector<float, VERTEX>&>( // downcast
					this->getVertex()[i]);
			}

		for (unsigned i = 0; i != this->getTriangleCount(); ++i)
		{
			this->surfaceNormals[i] = this->modelViewMatrix *
				static_cast<ThreeVector<float, NORMAL>&>( // downcast
					this->getSurfaceNormal()[i]);
		}
	}

	void RigidBody::effectElasticCollision( RigidBody& otherBody,
		ThreeVector< float>& pointOfCollision, ThreeVector< float>& normal)
	{
		// includes transformation to world coordinates
		ThreeVector< float> angularVelocity[2] = {
			this->modelViewMatrix * static_cast<ThreeVector< float, NORMAL>&&>(
					this->angularFrequency * this->rotationAxis),
			otherBody.modelViewMatrix * static_cast<ThreeVector< float, NORMAL>&&>(
					otherBody.angularFrequency * otherBody.rotationAxis)};

		ThreeVector< float> velocityAddend[2] = {
			normal / this->mass, - normal / otherBody.mass};

		ThreeVector< float> angularVelocityAddend[2] = {
			getCrossProduct( pointOfCollision -
				ThreeVector< float>( this->modelViewMatrix + 12), normal),
			- getCrossProduct( pointOfCollision -
				ThreeVector< float>( otherBody.modelViewMatrix + 12), normal)};

		angularVelocityAddend[0][0] /= this->momentOfInertia[0];
		angularVelocityAddend[0][1] /= this->momentOfInertia[1];
		angularVelocityAddend[0][2] /= this->momentOfInertia[2];

		angularVelocityAddend[1][0] /= otherBody.momentOfInertia[0];
		angularVelocityAddend[1][1] /= otherBody.momentOfInertia[1];
		angularVelocityAddend[1][2] /= otherBody.momentOfInertia[2];

		float commonFactor = -2.f * (this->velocity * normal - otherBody.velocity * normal +
				angularVelocity[0] * getCrossProduct( pointOfCollision -
					ThreeVector<float>( this->modelViewMatrix + 12), normal) -
				angularVelocity[1] * getCrossProduct( pointOfCollision -
					ThreeVector<float>( otherBody.modelViewMatrix + 12), normal)) /
			(normal * velocityAddend[0] - normal * velocityAddend[1] +
				getCrossProduct( pointOfCollision -
					ThreeVector<float>( this->modelViewMatrix + 12), normal) *
				angularVelocityAddend[0] -
				getCrossProduct( pointOfCollision -
					ThreeVector<float>( otherBody.modelViewMatrix + 12), normal) *
				angularVelocityAddend[1]);

		angularVelocity[0] += commonFactor * angularVelocityAddend[0];
		angularVelocity[1] += commonFactor * angularVelocityAddend[1];

		// transformation back to object coordinates
		static_cast< ThreeVector< float, NORMAL>&>( angularVelocity[0]).multiplyByInverse(
			this->modelViewMatrix);

		this->velocity += commonFactor * velocityAddend[0];

		otherBody.velocity += commonFactor * velocityAddend[1];

		this->angularFrequency = angularVelocity[0].getNorm();
		if (this->angularFrequency != .0f)
			this->rotationAxis = angularVelocity[0] / this->angularFrequency;

		otherBody.angularFrequency = angularVelocity[1].getNorm();
		if (otherBody.angularFrequency != .0f)
			otherBody.rotationAxis = angularVelocity[1] / otherBody.angularFrequency;
	}
}
