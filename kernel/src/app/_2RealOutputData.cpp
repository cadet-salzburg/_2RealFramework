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

using std::string;
using std::ostringstream;

namespace _2Real
{
	namespace app
	{
		AppData::AppData() :
			m_Data(),
			m_Typename( "void" ),
			m_Name( "undefined" )
		{
		}

		AppData::AppData( EngineData const& data, string const& type, string const& name ) :
			m_Data( data ),
			m_Typename( type ),
			m_Name( name )
		{
		}

		const string AppData::getLongTypename() const
		{
			return m_Data.getTypename();
		}

		string const& AppData::getName() const
		{
			return m_Name;
		}

		string const& AppData::getTypename() const
		{
			return m_Typename;
		}

		const string AppData::getDataAsString() const
		{
			ostringstream value;
			value << m_Data;
			return value.str();
		}
	}
}