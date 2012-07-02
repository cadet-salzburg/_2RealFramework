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

#include "helpers/_2RealEngineData.h"

#include <list>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>
#include <assert.h>
#include <iostream>

namespace _2Real
{

	class EngineImpl;

	class Typetable
	{

	public:

		template< typename Datatype >
		void registerType( std::string const& typeName );

		std::string const&		lookupTypename( std::string const& longTypename ) const;
		const std::string		lookupLongTypename( std::string const& typeName ) const;
		EngineData const&		getInitialValueFromLongTypename( std::string const& longTypename ) const;
		EngineData const&		getInitialValueFromTypename( std::string const& typeName ) const;

	private:

		typedef std::map< std::string, EngineData >		EngineDataTable;
		typedef std::map< std::string, std::string >	StringMap;

		EngineDataTable		m_Typetable;
		StringMap			m_LookupTable;

	};

	template< typename Datatype >
	void Typetable::registerType( std::string const& typeName )
	{

#ifdef _DEBUG
		typename EngineDataTable::iterator it = m_Typetable.find( typeName );
		if ( it != m_Typetable.end() )
		{
			std::cout << "keyword " << typeName << "already defined in typetable" << std::endl;
			assert( NULL );
		}
#endif

		std::string nameVec = std::string( "vector " + typeName );
		std::string nameList = std::string( "list " + typeName );

		Datatype data;
		std::vector< Datatype > vec;
		std::list< Datatype > list;

		EngineData simpleData( data );
		EngineData vectorData( vec );
		EngineData listData( list );

		m_Typetable[ typeName ] = simpleData;
		m_Typetable[ nameVec ] = vectorData;
		m_Typetable[ nameList ] = listData;

		std::string simpleType = typeid( Datatype ).name();
		std::string typeVec= vectorData.getTypename();
		std::string typeList = listData.getTypename();

		m_LookupTable[ simpleType ] = typeName;
		m_LookupTable[ typeVec ] = nameVec;
		m_LookupTable[ typeList ] = nameList;

	}

}