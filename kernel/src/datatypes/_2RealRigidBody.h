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

#include "datatypes/_2RealPoint.h"
#include "datatypes/_2RealQuaternion.h"

#include <string>
#include <sstream>
#include <vector>

namespace _2Real
{

	class RigidBody
	{
	// no serialization for now
	//friend std::ostream& operator<<( std::ostream& out, RigidBody const& data );
	//friend std::istream& operator>>( std::istream& in, RigidBody &data );

	public:

		RigidBody() {}
		RigidBody( RigidBody const& src ) : m_Points( src.m_Points ), m_Label( src.m_Label ), m_Id( src.m_Id ), m_ParentId( src.m_ParentId ), m_Position( src.m_Position ), m_Orientation ( src.m_Orientation ) {}
		RigidBody( const std::vector<_2Real::Point> points ) : m_Points( points ), m_Label( "undefined" ), m_Id( -1 ), m_ParentId( -1 ), m_Position ( _2Real::Point() ), m_Orientation ( _2Real::Quaternion() ) {}	
		RigidBody( const std::vector<_2Real::Point> points, std::string const& l, const unsigned int id, const unsigned int parentId, const _2Real::Point &position, const _2Real::Quaternion &orientation ) : m_Points( points ), m_Label( l ), m_Id( id ), m_ParentId( parentId ), m_Position( position ), m_Orientation( orientation ) {}
		
		bool operator==( RigidBody const& other ) const
		{
			return ( m_Id == other.m_Id && m_ParentId == other.m_ParentId && m_Label == other.m_Label && m_Position == other.m_Position && m_Points == other.m_Points );
		}

		std::vector<_2Real::Point>&	getPoints()	{ return m_Points; }

		void setLabel( std::string const& l )	{ m_Label = l; }
		std::string const& getLabel() const		{ return m_Label; }
		
		void setId( int id )					{ m_Id = id; }
		
		void setParentId( int id )				{ m_ParentId = id; }
		int getParentId() const					{ return m_ParentId; }
		
		void setPosition (Number x, Number y, Number z)	{ m_Position.setX(x); m_Position.setY(y); m_Position.setZ(z); }
		_2Real::Point getPosition() const		{ return m_Position; }
		
		void setOrientation (Number x, Number y, Number z, Number w) { m_Orientation.setX(x); m_Orientation.setY(y); m_Orientation.setZ(z); m_Orientation.setW(w); }
		_2Real::Quaternion getOrientation() const { return m_Orientation; }
		
		
	private:
		std::vector<_2Real::Point>		m_Points;
		std::string						m_Label;
		int								m_Id;
		int								m_ParentId;
		_2Real::Point					m_Position;
		_2Real::Quaternion				m_Orientation;

	};

	// no serialization for now
	//inline std::ostream& operator<<( std::ostream& out, RigidBody const& info )
	//{
	//	//out <<  << std::endl;
	//	return out;
	//}

	//inline std::istream& operator>>( std::istream& in, RigidBody &info )
	//{
	//	//in >> 
	//	return in;
	//}
}
