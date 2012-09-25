/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "datatypes/_2RealSkeleton.h"

namespace _2Real
{

	void Skeleton::updateBoundingBox()
	{
		m_BoundingBoxLimit.reset();
		_2Real::Point min = m_BoundingBoxLimit.getMin();
		_2Real::Point max = m_BoundingBoxLimit.getMax();

		for (std::vector< _2Real::RigidBody >::const_iterator it = m_RigidBodies.begin(); it != m_RigidBodies.end(); it++)
		{
			const _2Real::RigidBody rb = *it;
			const _2Real::Point pos = rb.getPosition();

			if (pos.x() > max.x()) max.setX(pos.x());
			if (pos.y() > max.y()) max.setY(pos.y());
			if (pos.z() > max.z()) max.setZ(pos.z());

			if (pos.x() < min.x()) min.setX(pos.x());
			if (pos.y() < min.y()) min.setY(pos.y());
			if (pos.z() < min.z()) min.setZ(pos.z());
		}
		m_BoundingBoxLimit.set(min, max);
	}
}