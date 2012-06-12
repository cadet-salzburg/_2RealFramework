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

#include "_2RealBlockIdentifier.h"
#include "_2RealEngineImpl.h"

namespace _2Real
{

	BlockIdentifier::BlockIdentifier() :
		m_Name( "invalid" ),
		m_Id( 0 )
	{
	}

	BlockIdentifier::BlockIdentifier( std::string const& name, const unsigned int id ) :
		m_Name( name ), m_Id( id )
	{
	}

	BlockIdentifier::BlockIdentifier( BlockIdentifier const& src ) :
		m_Name( src.m_Name ), m_Id( src.m_Id )
	{
	}

	BlockIdentifier& BlockIdentifier::operator=( BlockIdentifier const& src )
	{
		m_Name = src.m_Name;
		m_Id = src.m_Id;
		return *this;
	}

	bool BlockIdentifier::operator==( BlockIdentifier const& rhs ) const
	{
		return m_Id == rhs.m_Id;
	}

	bool BlockIdentifier::operator!=( BlockIdentifier const& rhs ) const
	{
		return m_Id != rhs.m_Id;
	}

	bool BlockIdentifier::operator<( BlockIdentifier const& rhs ) const
	{
		return m_Id < rhs.m_Id;
	}

	bool BlockIdentifier::operator<=( BlockIdentifier const& rhs ) const
	{
		return m_Id <= rhs.m_Id;
	}

	bool BlockIdentifier::operator>( BlockIdentifier const& rhs ) const
	{
		return m_Id > rhs.m_Id;
	}

	bool BlockIdentifier::operator>=( BlockIdentifier const& rhs ) const
	{
		return m_Id >= rhs.m_Id;
	}

}