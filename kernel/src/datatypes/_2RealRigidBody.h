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

	public:

		RigidBody() : m_hasPosition (false), m_hasOrientation (false), m_hasMarker (false) {}
		RigidBody( RigidBody const& src ) : m_Marker( src.m_Marker ), m_Label( src.m_Label ), m_Id( src.m_Id ), m_ParentId( src.m_ParentId ), m_Position( src.m_Position ), m_Orientation ( src.m_Orientation ), m_hasPosition (src.m_hasPosition), m_hasOrientation (src.m_hasOrientation), m_hasMarker(src.m_hasMarker) {}
		RigidBody( const std::vector<_2Real::Point> marker, std::string const& l, const unsigned int id, const unsigned int parentId, const _2Real::Point &position, const _2Real::Quaternion &orientation ) : m_Marker( marker ), m_Label( l ), m_Id( id ), m_ParentId( parentId ), m_Position( position ), m_Orientation( orientation ) , m_hasPosition (true), m_hasOrientation (true), m_hasMarker (true) {}
		
		bool operator==( RigidBody const& other ) const;

		void setMarker(std::vector<_2Real::Point>&	marker);
		std::vector<_2Real::Point>&	getMarker();

		void setLabel( std::string const& l );
		std::string const& getLabel() const;
		
		void setId( int id );
		int getId() const;
		
		void setParentId( int id );
		int getParentId() const;
		
		void setPosition (Number x, Number y, Number z);
		_2Real::Point getPosition() const;
		
		void setOrientation (Number x, Number y, Number z, Number w);
		_2Real::Quaternion getOrientation() const;
		
		bool hasPosition() const;
		bool hasOrientation() const;
		bool hasMarker() const;
		
	private:

		std::vector<_2Real::Point>		m_Marker;
		std::string						m_Label;
		int								m_Id;
		int								m_ParentId;
		_2Real::Point					m_Position;
		_2Real::Quaternion				m_Orientation;

		bool							m_hasPosition;
		bool							m_hasOrientation;
		bool							m_hasMarker;
	};
}
