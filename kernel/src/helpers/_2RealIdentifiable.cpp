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

#include "helpers/_2RealIdentifiable.h"

namespace _2Real
{
	AbstractIdentifiable::AbstractIdentifiable( Ids const& ids, std::string const& type, std::string const& name ) :
		m_Ids( ids ),
		m_Name( name ),
		m_Type( type )
	{
	}

	AbstractIdentifiable::~AbstractIdentifiable()
	{
	}

	const AbstractIdentifiable::Ids AbstractIdentifiable::getIds() const
	{
		Ids result = m_Ids;
		result.push_back( this );
		return result;
	}

	const std::string AbstractIdentifiable::getFullName() const
	{
		std::ostringstream name;
		for ( IdConstIterator it = m_Ids.begin(); it != m_Ids.end(); ++it )
		{
			//name << ( **it ).getType() << ": " << ( **it ).getName() << " - ";
			name << ( **it ).getName() << "::";
		}
		//name << m_Type << " : " << m_Name;
		name << m_Name;
		return name.str();
	}

	std::string const& AbstractIdentifiable::getName() const
	{
		return m_Name;
	}

	std::string const& AbstractIdentifiable::getType() const
	{
		return m_Type;
	}

	void AbstractIdentifiable::setName( std::string const& name )
	{
		m_Name = name;
	}
}