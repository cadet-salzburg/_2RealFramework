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
#include "datatypes/_2RealRigidBody.h"
#include "datatypes/_2RealBoundingBox.h"

#include <string>
#include <sstream>
#include <vector>

namespace _2Real
{

	class Skeleton
	{

	public:

		Skeleton() : m_Label("undefined"), m_Id(-1), m_Global(true) {}
		Skeleton( Skeleton const& src ) : m_RigidBodies( src.m_RigidBodies ), m_Label( src.m_Label ), m_Id( src.m_Id ), m_Global( src.m_Global ), m_BoundingBoxLimit(src.m_BoundingBoxLimit) {}
		Skeleton( const std::vector<_2Real::RigidBody> rigidBodies, std::string const& l, const unsigned int id, const bool global ) : m_RigidBodies( rigidBodies ), m_Label( l ), m_Id ( id ), m_Global( global ) {}
		Skeleton( const std::vector<_2Real::RigidBody> rigidBodies ) : m_RigidBodies( rigidBodies ), m_Label( "undefined" ), m_Id( -1 ), m_Global( true ) {}

		bool operator==( Skeleton const& other ) const
		{
			return ( m_Label == other.m_Label && m_Id == other.m_Id && m_BoundingBoxLimit == other.m_BoundingBoxLimit && m_RigidBodies == other.m_RigidBodies );
		}
		
		void setRigidBodies(std::vector<_2Real::RigidBody>&	rigidBodies)	{ m_RigidBodies = rigidBodies; updateBoundingBox(); }
		std::vector<_2Real::RigidBody>&	getRigidBodies()					{ return m_RigidBodies; }
	
		void setLabel( std::string const& l )								{ m_Label = l; }
		std::string const& getLabel() const									{ return m_Label; }
		
		void setId( int id )												{ m_Id = id; }
		int getId() const													{ return m_Id; }
		
		// global means that the positional and rotational skeleton data is relative to a global coordinate system
		void setGlobal( bool global)										{ m_Global = global; }
		bool isGlobal() const												{ return m_Global; }
		
		void setLimit(_2Real::BoundingBox bb)								{ m_BoundingBoxLimit = bb; }
		_2Real::BoundingBox getLimit()										{ return m_BoundingBoxLimit; }

		// calculates the bounding box limits based on the rigid body positions
		virtual void updateBoundingBox();

	private:

		std::vector< _2Real::RigidBody > m_RigidBodies;
		std::string						m_Label;
		int								m_Id;
		bool							m_Global;
		_2Real::BoundingBox				m_BoundingBoxLimit;			// space limits which describe e.g. for kinect the volume for world coordinates it's working, need this for the rendering in a 3d scen
	};

}
