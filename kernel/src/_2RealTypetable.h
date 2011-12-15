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

#pragma once

#include "_2RealEngineData.h"

#include <map>
#include <string>
#include <typeinfo>

namespace _2Real
{

	class Engine;

	typedef std::map< std::string, EngineData >		EngineDataTable;
	typedef std::map< std::string, std::string >	StringMap;

	class Typetable
	{

	public:

		Typetable(Engine const& engine);
		~Typetable();

		template< typename DataType >
		void				registerType(std::string const& keyword);

		const std::string	lookupKey(std::string const& type) const;
		const std::string	lookupType(std::string const& keyword) const;
		EngineData			getInitialValueFromType(std::string const& type) const;
		EngineData			getInitialValueFromKey(std::string const& keyword) const;
		bool				hasDefaultValue(std::string const& keyword) const;

	private:

		Engine				const& m_Engine;
		EngineDataTable		m_Typetable;
		StringMap			m_LookupTable;

	};

	template< typename DataType >
	void Typetable::registerType(std::string const& keyword)
	{
		EngineDataTable::iterator it = m_Typetable.find(keyword);
		if (it != m_Typetable.end())
		{
			std::ostringstream msg;
			msg << "keyword " << keyword << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		std::string keyword1 = std::string("vector " + keyword);
		std::string keyword2 = std::string("vector2D " + keyword);

		EngineData data(new DataType());
		EngineData data1(new std::vector< DataType >());
		EngineData data2(new std::vector < std::vector< DataType > >());

		m_Typetable[keyword] = data;
		m_Typetable[keyword1] = data1;
		m_Typetable[keyword2] = data2;

		std::string type = typeid(DataType).name();
		std::string type1 = data1.typeinfo().name();
		std::string type2 = data2.typeinfo().name();

		m_LookupTable[type] = keyword;
		m_LookupTable[type1] = keyword1;
		m_LookupTable[type2] = keyword2;
	}

}