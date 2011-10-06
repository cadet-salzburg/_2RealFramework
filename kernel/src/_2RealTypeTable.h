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

#include <map>
#include <string>
#include <vector>
#include <typeinfo>
#include <iostream>

namespace _2Real
{

	class TypeTable
	{

	public:

		TypeTable();

		std::map< std::string, std::string > getTable();

	private:

		template< typename T >
		void insertType(std::string name)
		{
			m_TypeTable[name] = typeid(T).name();
			m_TypeTable["vector " + name] = typeid(std::vector< T >).name();
			m_TypeTable["vector2D " + name] = typeid(std::vector< std::vector< T > >).name();
		}

		std::map< std::string, std::string >		m_TypeTable;

	};

}