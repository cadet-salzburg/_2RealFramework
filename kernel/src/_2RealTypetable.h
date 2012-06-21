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

#include <list>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>

namespace _2Real
{

	class EngineImpl;

	typedef std::map< std::string, EngineData >		EngineDataTable;
	typedef std::map< std::string, std::string >	StringMap;

	class Typetable
	{

	public:

		Typetable(EngineImpl const& EngineImpl);
		~Typetable();

		template< typename Datatype >
		void				registerType(std::string const& keyword);

		const std::string	lookupKey(std::string const& type) const;
		const std::string	lookupType(std::string const& keyword) const;
		EngineData			getInitialValueFromType(std::string const& type) const;
		EngineData			getInitialValueFromKey(std::string const& keyword) const;
		bool				hasDefaultValue(std::string const& keyword) const;

	private:

		EngineImpl				const& m_EngineImpl;
		EngineDataTable		m_Typetable;
		StringMap			m_LookupTable;

	};

	template< typename Datatype >
	void Typetable::registerType(std::string const& keyword)
	{
		EngineDataTable::iterator it = m_Typetable.find(keyword);
		if (it != m_Typetable.end())
		{
			std::ostringstream msg;
			msg << "keyword " << keyword << " already exists";
			throw AlreadyExistsException(msg.str());
		}

		//automatically also registers vector & list
		std::string keyword1 = std::string( "vector " + keyword );
		std::string keyword2 = std::string( "list " + keyword );

		EngineData data( new Datatype() );
		EngineData data1( new std::vector< Datatype >() );
		EngineData data2( new std::list< Datatype >() );

		m_Typetable[keyword] = data;
		m_Typetable[keyword1] = data1;
		m_Typetable[keyword2] = data2;

		std::string type = typeid( Datatype ).name();
		std::string type1 = data1.getTypename();
		std::string type2 = data2.getTypename();

		m_LookupTable[type] = keyword;
		m_LookupTable[type1] = keyword1;
		m_LookupTable[type2] = keyword2;
	}

}