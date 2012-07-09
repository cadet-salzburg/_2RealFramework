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

#include "engine/_2RealParameterData.h"

#include <sstream>
#include <iostream>

using std::string;
using std::ostream;
using std::ostringstream;

namespace _2Real
{

	ParameterData::ParameterData() :
		m_Name( "undefined" ),
		m_Typename( "undefined" ),
		m_LongTypename( "undefined" )
	{
	}

	ParameterData::ParameterData( string const& name, string const& longType, string const& type, Any const& defaultValue ) :
		m_Name( name ),
		m_Typename( type ),
		m_LongTypename( longType ),
		m_DefaultValue( defaultValue )
	{
	}

	string const& ParameterData::getName() const
	{
		return m_Name;
	}

	string const& ParameterData::getTypename() const
	{
		return m_Typename;
	}

	string const& ParameterData::getLongTypename() const
	{
		return m_LongTypename;
	}

	Any const& ParameterData::getDefaultValue() const
	{
		return m_DefaultValue;
	}

}