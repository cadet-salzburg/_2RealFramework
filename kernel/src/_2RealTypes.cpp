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

#include "_2RealTypes.h"

namespace _2Real
{

	TypeTable::TypeTable(Engine &_engine) :
		m_Types(),
		m_Engine(_engine)
	{
	}

	TypeTable::~TypeTable()
	{
	}

	SharedAny TypeTable::getDatatype(std::string const& _keyword) const
	{
		DatatypeTable::const_iterator it = m_Types.find(_keyword);

		if (it == m_Types.end())
		{
			throw Exception("internal exception ge datatype: keyword " + _keyword + " does not exist");
		}

		return it->second;
	}

	const std::string TypeTable::getTypename(std::string const& _keyword) const
	{
		DatatypeTable::const_iterator it = m_Types.find(_keyword);

		if (it == m_Types.end())
		{
			throw Exception("internal exception get typename: keyword " + _keyword + " does not exist");
		}

		return std::string(it->second.type().name());
	}

	const bool TypeTable::contains(std::string const& _keyword) const
	{
		DatatypeTable::const_iterator it = m_Types.find(_keyword);

		if (it == m_Types.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

}