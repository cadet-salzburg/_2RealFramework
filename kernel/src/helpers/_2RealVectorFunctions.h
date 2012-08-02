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

#pragma once

#include <datatypes/_2RealVector.h>

namespace _2Real
{
	inline const Vec3 &unitX()
	{
		static Vec3 x( 1.0f, 0.0f, 0.0f );
		return x;
	}

	inline const Vec3 &unitY()
	{
		static Vec3 y( 0.0f, 1.0f, 0.0f );
		return y;
	}

	inline const Vec3 &unitZ()
	{
		static Vec3 z( 0.0f, 0.0f, 1.0f );
		return z;
	}

	/*
	//TODO: implement matrix data type
	Vec3 rotateEulerXYZ( const Vec3 &v, const Vec3 &rot )
	{
		float s1 = sin( rot[0] );
		float s2 = sin( rot[1] );
		float s3 = sin( rot[2] );

		float c1 = cos( rot[0] );
		float c2 = cos( rot[1] );
		float c3 = cos( rot[2] );

		float r[] = {
			c2 * c3,	s1 * s2 * c3 + c1 * s3,	s1 * s3 - c1 * s2 * c3,
			-c2 * s3,	c1 * c3 - s1 * s2 * s3,	c1 * s2 * s3 + s1 * c3,
			s2,			-s1 * c2,				c1 * c2
		};

		peek::Mat3 R( r, peek::Mat3::Size );

		return Vector<T,3>( R * v );
	}
	*/
}