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

#include "engine/_2RealParameter.h"
#include "helpers/_2RealException.h"

#include <sstream>

using std::string;
using std::ostringstream;

namespace _2Real
{

	Parameter::Parameter( AbstractUberBlock &owningBlock, string const& name, string const& longTypename, string const& typeName ) :
		m_Owner( owningBlock ),
		m_Name( name ),
		m_LongTypename( longTypename ),
		m_Typename( typeName )
	{
	}

	std::string const& Parameter::getName() const
	{
		return m_Name;
	}

	std::string const& Parameter::getTypename() const
	{
		return m_Typename;
	}

	std::string const& Parameter::getLongTypename() const
	{
		return m_LongTypename;
	}

	AbstractUberBlock& Parameter::getOwningUberBlock()
	{
		return m_Owner;
	}

	void Parameter::setData( TimestampedData const& data )
	{
		if ( data.getData().getTypename() != m_LongTypename )
		{
			ostringstream msg;
			msg << "datatype mismatch: " << m_LongTypename << " vs. value type " << data.getData().getTypename();
			throw TypeMismatchException( msg.str() );
		}

		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_DataBuffer = data;
	}

	void Parameter::synchronize()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_Data = m_DataBuffer;
	}

	TimestampedData Parameter::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		return m_Data;
	}

}