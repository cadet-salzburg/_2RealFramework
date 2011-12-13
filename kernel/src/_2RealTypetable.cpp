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

#include "_2RealTypetable.h"
#include "_2RealException.h"
#include "_2RealEngine.h"

#include <sstream>

namespace _2Real
{

	Typetable::Typetable(Engine const& engine) :
		m_Engine(engine),
		m_Types(),
		m_LookupTable()
	{
	}

	Typetable::~Typetable()
	{
		m_Types.clear();
		m_LookupTable.clear();
	}

	const std::string Typetable::lookupType(std::string const& keyword) const
	{
		EngineDataTable::const_iterator it = m_Types.find(keyword);

		if (it == m_Types.end())
		{
			std::ostringstream msg;
			msg << "no type with keyword " << keyword << " found";
			throw InvalidTypeException(msg.str());
		}

		return std::string(it->second.typeinfo().name());
	}

	const std::string Typetable::lookupKey(std::string const& type) const
	{
		StringMap::const_iterator it = m_LookupTable.find(type);

		if (it == m_LookupTable.end())
		{
			std::ostringstream msg;
			msg << "no type with typename " << type << " found";
			throw InvalidTypeException(msg.str());
		}

		return it->second;
	}

	EngineData Typetable::getInitialValueFromType(std::string const& type) const
	{
		std::string keyword = lookupKey(type);
		return getInitialValueFromKey(keyword);
	}

	EngineData Typetable::getInitialValueFromKey(std::string const& keyword) const
	{
		EngineDataTable::const_iterator it = m_Types.find(keyword);

		if (it == m_Types.end())
		{
			std::ostringstream msg;
			msg << "no type with keyword " << keyword << " found";
			throw InvalidTypeException(msg.str());
		}

		return it->second;
	}

}