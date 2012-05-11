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

#include "_2RealEntity.h"

namespace _2Real
{

	IdCounter Entity::m_Counter = IdCounter();

	Entity::Entity(std::string const& name) :
		m_Id(m_Counter.getId()),
		m_Name(name)
	{
	}

	Entity::~Entity()
	{
	}

	Identifier Entity::getIdentifier() const
	{
		return Identifier(m_Name, m_Id);
	}

	const Identifier Entity::InvalidId()
	{
		return Identifier("invalid", 0);
	}

	std::string const& Entity::getName() const
	{
		return m_Name;
	}

}