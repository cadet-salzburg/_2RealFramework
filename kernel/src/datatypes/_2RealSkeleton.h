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

#include <string>
#include <sstream>
#include <vector>

namespace _2Real
{

	class Skeleton
	{
	// no serialization for now
	//friend std::ostream& operator<<( std::ostream& out, Skeleton const& data );
	//friend std::istream& operator>>( std::istream& in, Skeleton &data );

	public:

		Skeleton() {}
		Skeleton( Skeleton const& src ) : m_RigidBodies( src.m_RigidBodies ), m_Label( src.m_Label ), m_Id( src.m_Id ), m_Global( src.m_Global ) {}
		Skeleton( const std::vector<_2Real::RigidBody> rigidBodies, std::string const& l, const unsigned int id, const bool global ) : m_RigidBodies( rigidBodies ), m_Label( l ), m_Id ( id ), m_Global( global ) {}
		Skeleton( const std::vector<_2Real::RigidBody> rigidBodies ) : m_RigidBodies( rigidBodies ), m_Label( "undefined" ), m_Id( -1 ), m_Global( true ) {}
		
		std::vector<_2Real::RigidBody>&	getRigidBodies()	{ return m_RigidBodies; }
	
		void setLabel( std::string const& l )	{ m_Label = l; }
		std::string const& getLabel() const		{ return m_Label; }
		
		void setId( int id )					{ m_Id = id; }
		int getId() const						{ return m_Id; }
		
		void setGlobal( bool global)			{ m_Global = global; }
		bool isGlobal() const					{ return m_Global; }
		

	private:
		std::vector<_2Real::RigidBody>  m_RigidBodies;
		std::string						m_Label;
		int								m_Id;
		bool							m_Global;
	};

	// no serialization for now
	//inline std::ostream& operator<<( std::ostream& out, Skeleton const& info )
	//{
	//	//out <<  << std::endl;
	//	return out;
	//}

	//inline std::istream& operator>>( std::istream& in, Skeleton &info )
	//{
	//	//in >> 
	//	return in;
	//}

}
