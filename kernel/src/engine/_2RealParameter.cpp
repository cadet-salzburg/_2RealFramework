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
using std::stringstream;

namespace _2Real
{

	Parameter::Parameter( TypeDescriptor const& type ) :
		m_Descriptor( type )
	{
	}

	std::string const& Parameter::getTypename() const
	{
		return m_Descriptor.m_TypeName;
	}

	const std::string Parameter::getLongTypename() const
	{
		return m_Descriptor.m_LongTypename;
	}

	Type const& Parameter::getType() const
	{
		return m_Descriptor.m_Type;
	}

	TypeCategory const& Parameter::getTypeCategory() const
	{
		return m_Descriptor.m_TypeCategory;
	}

	void Parameter::setData( TimestampedData const& data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		//if ( data.getData().getLongTypename() != m_LongTypename )
		//{
		//	ostringstream msg;
		//	msg << "datatype mismatch: " << m_LongTypename << " vs. value type " << data.getData().getLongTypename();
		//	throw TypeMismatchException( msg.str() );
		//}

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