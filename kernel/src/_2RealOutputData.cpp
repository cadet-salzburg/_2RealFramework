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

#include "_2RealOutputData.h"

#include <sstream>

namespace _2Real
{

	OutputData::OutputData() : m_Data(), m_Typename() {}

	OutputData::OutputData( EngineData const& data, std::string const& type, std::string const& name ) :
		m_Data( data ),
		m_Typename( type ),
		m_Name( name )
	{
	}

	const std::string OutputData::getLongTypename() const
	{
		return m_Data.getTypeinfo().name();
	}

	std::string const& OutputData::getName() const
	{
		return m_Name;
	}

	std::string const& OutputData::getTypename() const
	{
		return m_Typename;
	}

	const std::string OutputData::getDataAsString() const
	{
		std::ostringstream value;
		value << m_Data;
		return value.str();
	}

}