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

#include "_2RealIdentities.h"
#include "_2RealErrorState.h"

#include <sstream>
#include <stdint.h>

namespace _2Real
{

	Identities::Identities() : m_iCreationCount(0)
	{
	}

	Identities::Identities(Identities const& _src) throw(...)
	{
		throw ErrorState::failure();
	}

	Identities& Identities::operator=(Identities const& _src) throw(...)
	{
		throw ErrorState::failure();
	}

	Identities::~Identities()
	{
		for (IDMap::iterator it = m_IDs.begin(); it != m_IDs.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	IdentifierImpl const *const Identities::createID(std::string const& _name, IdentifierImpl::eType const& _type) throw(...)
	{
		std::stringstream tmp;
		tmp << ++m_iCreationCount;
		std::string count = tmp.str();

		std::string name;
		std::string type;
		
		switch(_type)
		{

		//plugin = loaded dll
		case IdentifierImpl::PLUGIN:
			type = "plugin";
			break;

		//container wrapping user defined service
		case IdentifierImpl::SERVICE:
			type = "service";
			break;

		//container wrapping user defined service
		case IdentifierImpl::FACTORY:
			type = "service factory";
			break;

		default:
			throw ErrorState::failure();
			break;

		}

		IdentifierImpl *id = new IdentifierImpl(_name, type, _type, m_iCreationCount);
		m_IDs.insert(NamedID(id->id(), id));
		return id;
	}

}