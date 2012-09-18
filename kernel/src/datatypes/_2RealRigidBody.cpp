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

#include "datatypes/_2RealRigidBody.h"

namespace _2Real
{

	bool RigidBody::operator==( RigidBody const& other ) const
	{
		return ( m_Id == other.m_Id && m_ParentId == other.m_ParentId && m_Label == other.m_Label && m_Position == other.m_Position && m_Marker == other.m_Marker );
	}

	void RigidBody::setMarker(std::vector<_2Real::Point>&	marker)	
	{ 
		m_Marker = marker; 
		m_hasMarker = true; 
	}

	std::vector<_2Real::Point>&	RigidBody::getMarker()	
	{ 
		return m_Marker; 
	}

	void RigidBody::setLabel( std::string const& l )	
	{ 
		m_Label = l; 
	}

	std::string const& RigidBody::getLabel() const		
	{ 
		return m_Label; 
	}

	void RigidBody::setId( int id )					
	{ 
		m_Id = id; 
	}

	int RigidBody::getId() const
	{ 
		return m_Id; 
	}

	void RigidBody::setParentId( int id )
	{
		m_ParentId = id; 
	}

	int RigidBody::getParentId() const				
	{
		return m_ParentId;
	}

	void RigidBody::setPosition (Number x, Number y, Number z)
	{
		m_Position.setX(x);
		m_Position.setY(y); 
		m_Position.setZ(z); 
		m_hasPosition = true;
	}

	_2Real::Point RigidBody::getPosition() const	
	{ 
		return m_Position; 
	}

	void RigidBody::setOrientation (Number x, Number y, Number z, Number w) 
	{ 
		m_Orientation.x() = x; 
		m_Orientation.y() = y;
		m_Orientation.z() = z; 
		m_Orientation.w() = w; 
		m_hasOrientation = true;
	}

	_2Real::Quaternion RigidBody::getOrientation() const 
	{ 
		return m_Orientation;
	}

	bool RigidBody::hasPosition() const			
	{
		return m_hasPosition;
	}

	bool RigidBody::hasOrientation() const			
	{
		return m_hasOrientation;
	}

	bool RigidBody::hasMarker() const		
	{
		return m_hasMarker;
	}
}