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

#include "_2RealEntities.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

#include <sstream>
#include <stdint.h>

namespace _2Real
{

	Entities::Entities() : m_iCreationCount(0)
	{
	}

	Entities::Entities(Entities const& _src) throw(...)
	{
		throw Exception::failure();
	}

	Entities& Entities::operator=(Entities const& _src) throw(...)
	{
		throw Exception::failure();
	}

	Entities::~Entities()
	{
		for (EntityMap::iterator it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}

		m_Entities.clear();
	}

	IdentifierImpl const *const Entities::createID(std::string const& _name, IEntity *const _entity) throw(...)
	{
		std::stringstream tmp;
		tmp << ++m_iCreationCount;
		std::string count = tmp.str();

		std::string name;
		std::string type;
		
		switch(_entity->type())
		{

		//plugin = loaded dll
		case IEntity::PLUGIN:
			type = "plugin";
			break;

		//container wrapping user defined service
		case IEntity::SERVICE:
			type = "service";
			break;

		//container wrapping user defined service
		case IEntity::FACTORY:
			type = "service factory";
			break;

		default:
			throw Exception::failure();
			break;

		}

		IdentifierImpl *id = new IdentifierImpl(_name, type, _entity->type(), m_iCreationCount);
		m_Entities.insert(NamedEntity(*id, _entity));
		return id;
	}

	IEntity *const Entities::get(IdentifierImpl const *const _id) throw(...)
	{
		EntityMap::iterator it = m_Entities.find(*_id);

		if (it == m_Entities.end())
		{
			throw Exception::failure();
		}

		return it->second;
	}

}