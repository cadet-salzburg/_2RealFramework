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

#include "datatypes/_2RealVector.h"

#include <istream>
#include <ostream>
#include <string>

namespace _2Real
{
	class BoundingBox
	{

	public:

		BoundingBox() : m_Min( 0 ), m_Max( 0 ) {}
		BoundingBox( Vec3 const& min, Vec3 const& max ) : m_Min( min ), m_Max( max ) {}
		BoundingBox( BoundingBox const& src ) : m_Min( src.m_Min ), m_Max( src.m_Max ) {}

		void set( Vec3 const& min, Vec3 const& max)
		{
			m_Min = min;
			m_Max = max;
		}

		const Vec3& getMin()	{ return m_Min; }
		const Vec3& getMax()	{ return m_Max; }

	private:

		_2Real::Vec3	m_Min;
		_2Real::Vec3	m_Max;

	};
}
