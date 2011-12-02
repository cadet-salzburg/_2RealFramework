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

#include "_2RealException.h"
#include "_2RealSharedAny.h"
#include "_2RealEngine.h"

#include <map>

namespace _2Real
{

	class TypeTable
	{

	public:

		TypeTable(Engine &engine);
		virtual ~TypeTable();

		template< typename T >
		void registerType(std::string const& keyword)
		{
			DatatypeTable::iterator it = m_Types.find(keyword);

			if (it != m_Types.end())
			{
				throw Exception("keyword already exists");
			}

			std::string key1 = std::string("vector " + keyword);
			std::string key2 = std::string("vector2D " + keyword);

			m_Types[keyword] = SharedAny(new T());
			m_Types[key1] = SharedAny(new std::vector< T >());
			m_Types[key2] = SharedAny(new std::vector < std::vector< T > >());
		}

		void create(std::string const& keyword, SharedAny &any) const
		{
			DatatypeTable::const_iterator it = m_Types.find(keyword);

			if (it == m_Types.end())
			{
				throw Exception("keyword does not exist");
			}

			any.create(it->second);
		}

		SharedAny getDatatype(std::string const& keyword) const;
		const std::string getTypename(std::string const& keyword) const;
		const bool contains(std::string const& keyword) const;

	private:

		typedef std::map< std::string, SharedAny >		DatatypeTable;

		DatatypeTable									m_Types;
		Engine											&m_Engine;

	};

}