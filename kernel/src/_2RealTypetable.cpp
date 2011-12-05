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
		m_Engine(engine),	//do not touch anywhere else in ctor
		m_Types(),
		m_LookupTable()
	{
	}

	Typetable::~Typetable()
	{
		m_Types.clear();
		m_LookupTable.clear();
	}

	void Typetable::createEngineData(std::string const& keyword, EngineData &any) const
	{
		EngineDataTable::const_iterator it = m_Types.find(keyword);

		if (it == m_Types.end())
		{
			std::ostringstream msg;
			msg << "typetable error: createEngineData, keyword not contained in table: " << keyword << std::endl;
			throw _2Real::Exception(msg.str());
		}

		any.create(it->second);
	}

	EngineData Typetable::getEngineData(std::string const& keyword) const
	{
		EngineDataTable::const_iterator it = m_Types.find(keyword);

		if (it == m_Types.end())
		{
			std::ostringstream msg;
			msg << "typetable error: getEngineData, keyword not contained in table: " << keyword << std::endl;
			throw _2Real::Exception(msg.str());
		}

		return it->second;
	}

	const std::string Typetable::getTypename(std::string const& keyword) const
	{
		EngineDataTable::const_iterator it = m_Types.find(keyword);

		if (it == m_Types.end())
		{
			std::ostringstream msg;
			msg << "typetable error: getTypename, keyword not contained in table: " << keyword << std::endl;
			throw _2Real::Exception(msg.str());
		}

		return std::string(it->second.type().name());
	}

	const bool Typetable::contains(std::string const& keyword) const
	{
		return m_Types.find(keyword) != m_Types.end();
	}

}