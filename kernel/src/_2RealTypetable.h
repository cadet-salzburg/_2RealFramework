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

		template< typename Datatype >
		void				registerType(std::string const& keyword);
		void				createEngineData(std::string const& keyword, EngineData &any) const;
		EngineData			getEngineData(std::string const& keyword) const;
		const std::string	getTypename(std::string const& keyword) const;
		const bool			contains(std::string const& keyword) const;
		StringMap const&	getLookupTable() const;

	private:

		Engine				const& m_Engine;
		EngineDataTable		m_Types;
		StringMap			m_LookupTable;

	};

	template< typename Datatype >
	void Typetable::registerType(std::string const& keyword)
	{
		EngineDataTable::iterator it = m_Types.find(keyword);

		if (it != m_Types.end())
		{
			throw Exception("keyword already exists");
		}

		std::string keyword1 = std::string("vector " + keyword);
		std::string keyword2 = std::string("vector2D " + keyword);

		EngineData data(new Datatype());
		EngineData data1(new std::vector< Datatype >());
		EngineData data2(new std::vector < std::vector< Datatype > >());

		m_Types[keyword] = data;
		m_Types[keyword1] = data1;
		m_Types[keyword2] = data2;

		m_LookupTable[keyword] = data.type().name();
		m_LookupTable[keyword1] = data1.type().name();
		m_LookupTable[keyword2] = data2.type().name();
	}

	inline StringMap const& Typetable::getLookupTable() const
	{
		return m_LookupTable;
	}

}