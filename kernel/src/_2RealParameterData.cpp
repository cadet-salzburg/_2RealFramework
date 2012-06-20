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

#include "_2RealParameterData.h"

#include <sstream>
#include <iostream>

using std::string;

namespace _2Real
{

	ParameterData::ParameterData() :
		m_Name( "undefined" ),
		m_Typename( "undefined" ),
		m_LongTypename( "undefined" ),
		m_Category( "undefined" )
	{
	}

	ParameterData::ParameterData( string const& name, string const& longType, string const& type, string const& category, EngineData const& defaultValue ) :
		m_Name( name ),
		m_Typename( type ),
		m_LongTypename( longType ),
		m_Category( category ),
		m_DefaultValue( defaultValue )
	{
	}

	std::string const& ParameterData::getName() const
	{
		return m_Name;
	}

	std::string const& ParameterData::getTypename() const
	{
		return m_Typename;
	}

	std::string const& ParameterData::getLongTypename() const
	{
		return m_LongTypename;
	}

	std::string const& ParameterData::getCategory() const
	{
		return m_Category;
	}

	EngineData const& ParameterData::getDefaultValue() const
	{
		return m_DefaultValue;
	}

	std::ostream& operator<<( std::ostream &out, ParameterData const& metadata )
	{
		out << metadata.getName() << "\t" << metadata.getTypename();
		out << ", default value: " << metadata.getDefaultValue();
		return out;
	}

}