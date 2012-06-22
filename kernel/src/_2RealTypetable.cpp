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

#include <sstream>

using std::string;
using std::ostringstream;

namespace _2Real
{

	const string Typetable::lookupLongTypename( string const& typeName ) const
	{
		EngineDataTable::const_iterator it = m_Typetable.find( typeName );

		if ( it == m_Typetable.end() )
		{
			ostringstream msg;
			msg << typeName << " is not known by the engine";
			throw InvalidTypeException( msg.str() );
		}

		return it->second.getTypename();
	}

	string const& Typetable::lookupTypename( string const& longTypename ) const
	{
		StringMap::const_iterator it = m_LookupTable.find( longTypename );

		if ( it == m_LookupTable.end() )
		{
			ostringstream msg;
			msg << longTypename << " is not a valid datatype in the engine";
			throw InvalidTypeException( msg.str() );
		}

		return it->second;
	}

	EngineData const& Typetable::getInitialValueFromLongTypename( string const& longTypename ) const
	{
		string typeName = lookupTypename( longTypename );
		return getInitialValueFromTypename( typeName );
	}

	EngineData const& Typetable::getInitialValueFromTypename( string const& typeName ) const
	{
		EngineDataTable::const_iterator it = m_Typetable.find( typeName );

		if ( it == m_Typetable.end() )
		{
			ostringstream msg;
			msg << typeName << " is not known by the engine";
			throw InvalidTypeException( msg.str() );
		}

		return it->second;
	}

}