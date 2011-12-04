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

#include "Poco/Mutex.h"

#include <iostream>

namespace _2Real
{

	class IdCounter
	{

	public:

		IdCounter() :
			m_Counter(0)
		{
		}

		unsigned int getId()
		{
			Poco::FastMutex::ScopedLock lock(m_Mutex);
			return ++m_Counter;
		}

	private:

		Poco::FastMutex	m_Mutex;
		unsigned int	m_Counter;

	};

	const Identifier Entity::createIdentifier(std::string const& name, std::string const& type)
	{
		static IdCounter counter;
		return Identifier(name, type, counter.getId());
	}

	Entity::Entity(Identifier const& id) :
		m_Id(id)
	{
		std::cout << "creating entity " << m_Id << std::endl;
	}

	Entity::~Entity()
	{
	}

	const Identifier Entity::NoEntity()
	{
		return Identifier("", "", 0);
	}

}