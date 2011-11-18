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
#include "_2RealException.h"
#include "_2RealId.h"

#include <iostream>

namespace _2Real
{

	Entity::Entity(Id *const _id) :
		m_ID(_id)
	{
		if (_id == NULL)
		{
			throw Exception("internal error - entity creation with null pointer");
		}
	}

	Entity::~Entity()
	{
		if (m_ID)
		{
			m_ID->release();
		}
	}

	unsigned int const& Entity::id() const
	{
		if (m_ID != NULL)
		{
			return m_ID->id();
		}

		throw Exception("internal error - identifier of entity is null");
	}

	std::string const& Entity::name() const
	{
		if (m_ID != NULL)
		{
			return m_ID->name();
		}

		throw Exception("internal error - identifier of entity is null");
	}
	
	std::string const& Entity::info() const
	{
		if (m_ID != NULL)
		{
			return m_ID->info();
		}

		throw Exception("internal error - identifier of entity is null");
	}

	void Entity::setInfo(std::string const& _info)
	{
		if (m_ID != NULL)
		{
			m_ID->setInfo(_info);
			return;
		}

		throw Exception("internal error - identifier of entity is null");
	}

}