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
#include <iostream>
#include <string>

namespace _2Real
{
	class BoundingBox
	{

	public:

		BoundingBox() : m_Min(  ), m_Max( ) {}
		BoundingBox( Point min, Point max ) : m_Min( min ), m_Max( max ) {}
		BoundingBox( BoundingBox const& src ) : m_Min( src.m_Min ), m_Max( src.m_Max ) {}
		BoundingBox & BoundingBox::operator=( BoundingBox const& src ) { m_Min = src.m_Min; m_Max = src.m_Max; return *this; }

		void set( const Point& min, const Point& max)
		{
			m_Min = min;
			m_Max = max;
		}

		Point getMin()	{ return m_Min; }
		Point getMax()	{ return m_Max; }

		bool operator==(BoundingBox const& rhs) const { return (m_Min == rhs.m_Min && m_Max == rhs.m_Max); }
		bool operator!=(BoundingBox const& rhs) const { return !(*this == rhs); }

	private:

		_2Real::Point	m_Min;
		_2Real::Point	m_Max;

	};
}
