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

#include "app/_2RealAppData.h"
#include "engine/_2RealTimestampedData.h"

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

		AppData::AppData( TimestampedData const& data, string const& typeName, std::string const& longTypename, string const& name ) :
			m_Data( data.anyValue ),
			m_Typename( typeName ),
			m_LongTypename( longTypename ),
			m_Name( name )
		{
		}

		const string AppData::getLongTypename() const
		{
			return m_LongTypename;
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
			ostringstream str;
			m_Data.writeTo( str );
			return str.str();
		}
	}
}