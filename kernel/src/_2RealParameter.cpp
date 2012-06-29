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

#include "_2RealParameter.h"
#include "_2RealParameterData.h"
#include "_2RealException.h"

#include <sstream>

using std::string;
using std::ostringstream;

namespace _2Real
{

	Parameter::Parameter( AbstractUberBlock &owner, string const& name, string const& longTypename, string const& typeName ) :
		Param( owner, name, longTypename, typeName )
	{
	}

	void Parameter::setData( TimestampedData const& data )
	{
		if ( data.getData().getTypename() != Param::getLongTypename() )
		{
			ostringstream msg;
			msg << "datatype mismatch: " << Param::getLongTypename() << " vs. value type " << data.getData().getTypename();
			throw TypeMismatchException( msg.str() );
		}

		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_WriteData = data;
	}

	void Parameter::synchronize()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_Data = m_WriteData;
	}

	EngineData Parameter::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		return m_Data.getData();
	}

}