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

#include "datatypes/_2RealNumber.h"

#include <istream>
#include <ostream>
#include <string>

namespace _2Real
{
	class Quaternion
	{

		friend std::istream & operator>>( std::istream &in, Quaternion &number );
		friend std::ostream & operator<<( std::ostream &out, Quaternion const& number );

	public:

		Quaternion() : m_X( 0 ), m_Y( 0 ), m_Z( 0 ), m_W( 0 ), m_Label( "undefined" ), m_Id( -1 ) {}
		Quaternion( Number const& x, Number const& y, Number const& z, Number const& w ) : m_X( x ), m_Y( y ), m_Z( z ), m_W ( w ), m_Label( "undefined" ), m_Id( -1 ) {}
		Quaternion( Number const& x, Number const& y, Number const& z, Number const& w, std::string const& l, const unsigned int id ) : m_X( x ), m_Y( y ), m_Z( z ), m_W( w ), m_Label( l ), m_Id( id ) {}
		Quaternion( Quaternion const& src ) : m_X( src.m_X ), m_Y( src.m_Y ), m_Z( src.m_Z ), m_W( src.m_W ), m_Label( src.m_Label ), m_Id( src.m_Id ) {}

		void set(  Number const& x, Number const& y, Number const& z, Number const& w, std::string const& l, const unsigned int id )
		{
			m_X = x;
			m_Y = y;
			m_Z = z;
			m_W = w;
			m_Label = l;
			m_Id = id;
		}

		void setX(  Number const& x )
		{
			m_X = x;
		}

		void setY(  Number const& y )
		{
			m_Y = y;
		}

		void setZ(  Number const& z )
		{
			m_Z = z;
		}
		
		void setW(  Number const& w )
		{
			m_W = w;
		}

		void setLabel( std::string const& l )	{ m_Label = l; }
		std::string const& getLabel() const		{ return m_Label; }
		void setId( int id )					{ m_Id = id; }
		int getId() const						{ return m_Id; }

		Number & x() { return m_X; }
		Number & y() { return m_Y; }
		Number & z() { return m_Z; }
		Number & w() { return m_W; }
		Number const& x() const { return m_X; }
		Number const& y() const { return m_Y; }
		Number const& z() const { return m_Z; }
		Number const& w() const { return m_W; }

	private:

		Number			m_X;
		Number			m_Y;
		Number			m_Z;
		Number 			m_W;
		std::string		m_Label;
		int				m_Id;

	};
}
