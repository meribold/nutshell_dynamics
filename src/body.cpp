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

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef> // std::size_t

#include "body.h"

namespace nut
{
	Body::Body(ThreeVector<float> vertices[], ThreeVector<float> surfaceNormals[],
		const Body::Pool& pool) :
		vertices{vertices}, surfaceNormals{surfaceNormals}, pool(pool) {}

	inline std::array<ThreeVector<float>, 2>*
	Body::doesCollide(const Body(& body)[2], const unsigned(& faceIndex)[2])
	{
		return body[0].doesCollide(body[1], faceIndex[0], faceIndex[1]);
	}

	inline std::array<ThreeVector<float>, 2>*
	Body::doesCollide(const Body& otherBody, unsigned faceIdentifier,
		unsigned otherIdentifier) const
	{
		const ThreeVector<float>* const faces[2][3] = {{
				&this->vertices[this->getFaces()[faceIdentifier][0]],
				&this->vertices[this->getFaces()[faceIdentifier][1]],
				&this->vertices[this->getFaces()[faceIdentifier][2]]},
			{
				&otherBody.vertices[otherBody.getFaces()[otherIdentifier][0]],
				&otherBody.vertices[otherBody.getFaces()[otherIdentifier][1]],
				&otherBody.vertices[otherBody.getFaces()[otherIdentifier][2]]}
		};

		const ThreeVector<float>* const surfaceNormals[2] = {
			&this->surfaceNormals[faceIdentifier], &otherBody.surfaceNormals[otherIdentifier]
		};

		float distance[2][3];

		// Compute the minimal distance from all of the first face's vertices to the second
		// face using the Hesse normal form.
		distance[0][0] = (*faces[1][0] - *faces[0][0]) * *surfaceNormals[1];
		distance[0][1] = (*faces[1][0] - *faces[0][1]) * *surfaceNormals[1];
		distance[0][2] = (*faces[1][0] - *faces[0][2]) * *surfaceNormals[1];

		int separateVertex[2];

		if (distance[0][0] < .0f)
			if (distance[0][1] < .0f)
				if (distance[0][2] < .0f)
					return nullptr; // No collision.
				else
					// Vertex 2 is separated from 0 and 1 by the other triangle i.e.  on the other
					// side of that triangle.
					separateVertex[0] = 2;
			else
				if (distance[0][2] < .0f)
					separateVertex[0] = 1;
				else
					separateVertex[0] = 0;
		else
			if (distance[0][1] < .0f)
				if (distance[0][2] < .0f)
					separateVertex[0] = 0;
				else
					separateVertex[0] = 1;
			else
				if (distance[0][2] < .0f)
					separateVertex[0] = 2;
				else
					return nullptr; // No collision.

		// Compute the minimal distance from all of the second faces vertices to the first
		// face.
		distance[1][0] = (*faces[0][0] - *faces[1][0]) * *surfaceNormals[0];
		distance[1][1] = (*faces[0][0] - *faces[1][1]) * *surfaceNormals[0];
		distance[1][2] = (*faces[0][0] - *faces[1][2]) * *surfaceNormals[0];

		if (distance[1][0] < .0f)
			if (distance[1][1] < .0f)
				if (distance[1][2] < .0f)
					return nullptr; // No collision.
				else
					separateVertex[1] = 2;
			else
				if (distance[1][2] < .0f)
					separateVertex[1] = 1;
				else
					separateVertex[1] = 0;
		else
			if (distance[1][1] < .0f)
				if (distance[1][2] < .0f)
					separateVertex[1] = 0;
				else
					separateVertex[1] = 1;
			else
				if (distance[1][2] < .0f)
					separateVertex[1] = 2;
				else
					return nullptr; // No collision.

		ThreeVector<float> lineOfIntersection =
			getCrossProduct(*surfaceNormals[0], *surfaceNormals[1]);

		std::size_t keyDimension;

		if (std::fabs(lineOfIntersection[0]) < std::fabs(lineOfIntersection[1]))
			keyDimension = std::fabs(lineOfIntersection[1]) < std::fabs(lineOfIntersection[2]) ?
				2 : 1;
		else
			keyDimension = std::fabs(lineOfIntersection[0]) < std::fabs(lineOfIntersection[2]) ?
				2 : 0;

		struct {	ThreeVector<float> endPoint[2]; } lineSegment[2];

		lineSegment[0].endPoint[0] =
			(*faces[0][separateVertex[0]] *
			std::fabs(distance[0][(separateVertex[0] + 1) % 3]) +
			*faces[0][(separateVertex[0] + 1) % 3] *
			std::fabs(distance[0][separateVertex[0]])) /
			(std::fabs(distance[0][separateVertex[0]]) +
			std::fabs(distance[0][(separateVertex[0] + 1) % 3]));

		lineSegment[0].endPoint[1] =
			(*faces[0][separateVertex[0]] *
			std::fabs(distance[0][(separateVertex[0] + 2) % 3]) +
			*faces[0][(separateVertex[0] + 2) % 3] *
			std::fabs(distance[0][separateVertex[0]])) /
			(std::fabs(distance[0][separateVertex[0]]) +
			std::fabs(distance[0][(separateVertex[0] + 2) % 3]));

		lineSegment[1].endPoint[0] =
			(*faces[1][separateVertex[1]] *
			std::fabs(distance[1][(separateVertex[1] + 1) % 3]) +
			*faces[1][(separateVertex[1] + 1) % 3] *
			std::fabs(distance[1][separateVertex[1]])) /
			(std::fabs(distance[1][separateVertex[1]]) +
			std::fabs(distance[1][(separateVertex[1] + 1) % 3]));

		lineSegment[1].endPoint[1] =
			(*faces[1][separateVertex[1]] *
			std::fabs(distance[1][(separateVertex[1] + 2) % 3]) +
			*faces[1][(separateVertex[1] + 2) % 3] *
			std::fabs(distance[1][separateVertex[1]])) /
			(std::fabs(distance[1][separateVertex[1]]) +
			std::fabs(distance[1][(separateVertex[1] + 2) % 3]));

		if ((lineSegment[0].endPoint[0][keyDimension] <=
			lineSegment[1].endPoint[0][keyDimension]) ^
			(lineSegment[0].endPoint[0][keyDimension] <
			lineSegment[1].endPoint[1][keyDimension]))
		{
			if ((lineSegment[0].endPoint[1][keyDimension] <=
				lineSegment[1].endPoint[0][keyDimension]) ^
				(lineSegment[0].endPoint[1][keyDimension] <
				lineSegment[1].endPoint[1][keyDimension]))
			{
				return new std::array<ThreeVector<float>, 2>{{
					ThreeVector<float>(*faces[0][separateVertex[0]]),
					ThreeVector<float>(*surfaceNormals[1])}};
			}
			else if ((lineSegment[1].endPoint[0][keyDimension] <=
				lineSegment[0].endPoint[0][keyDimension]) ^
				(lineSegment[1].endPoint[0][keyDimension] <
				lineSegment[0].endPoint[1][keyDimension]))
			{
				return new std::array<ThreeVector<float>, 2>{{
					lineSegment[0].endPoint[0],
					getCrossProduct(*faces[0][separateVertex[0]] -
					                *faces[0][(separateVertex[0] + 1) % 3],
					                *faces[1][separateVertex[1]] -
					                *faces[1][(separateVertex[1] + 1) % 3]).getUnitVector()}};
			}
			else
			{
				return new std::array<ThreeVector<float>, 2>{{
					lineSegment[0].endPoint[0],
					getCrossProduct(*faces[0][separateVertex[0]] -
					                *faces[0][(separateVertex[0] + 1) % 3],
					                *faces[1][separateVertex[1]] -
					                *faces[1][(separateVertex[1] + 2) % 3]).getUnitVector()}};
			}
		}
		else if ((lineSegment[0].endPoint[1][keyDimension] <=
			lineSegment[1].endPoint[0][keyDimension]) ^
			(lineSegment[0].endPoint[1][keyDimension] <
			lineSegment[1].endPoint[1][keyDimension]))
		{
			if ((lineSegment[1].endPoint[0][keyDimension] <=
				lineSegment[0].endPoint[0][keyDimension]) ^
				(lineSegment[1].endPoint[0][keyDimension] <
				lineSegment[0].endPoint[1][keyDimension]))
			{
				return new std::array<ThreeVector<float>, 2>{{
					lineSegment[0].endPoint[1],
					getCrossProduct(*faces[0][separateVertex[0]] -
					                *faces[0][(separateVertex[0] + 2) % 3],
					                *faces[1][separateVertex[1]] -
					                *faces[1][(separateVertex[1] + 1) % 3]).getUnitVector()}};
			}
			else
			{
				return new std::array<ThreeVector<float>, 2>{{
					lineSegment[0].endPoint[1],
					getCrossProduct(*faces[0][separateVertex[0]] -
					                *faces[0][(separateVertex[0] + 2) % 3],
					                *faces[1][separateVertex[1]] -
					                *faces[1][(separateVertex[1] + 2) % 3]).getUnitVector()}};
			}
		}
		else if ((lineSegment[1].endPoint[0][keyDimension] <=
			lineSegment[0].endPoint[0][keyDimension]) ^
			(lineSegment[1].endPoint[0][keyDimension] <
			lineSegment[0].endPoint[1][keyDimension]))
		{
			return new std::array<ThreeVector<float>, 2>{{
				ThreeVector<float>(*faces[1][separateVertex[1]]),
				ThreeVector<float>(*surfaceNormals[0])}};
		}
		return nullptr;
	}

	std::array<ThreeVector<float>, 2>* Body::doesCollide(const Body& otherBody) const
	{
		for (unsigned i = 0; i != this->getTriangleCount(); ++i)
		{
			for (unsigned j = 0; j != otherBody.getTriangleCount(); ++j)
			{
				// We take the first hit.
				if (auto partialCollisionContext = this->doesCollide(otherBody, i, j))
					return partialCollisionContext;
			}
		}
		return nullptr;
	}
}

