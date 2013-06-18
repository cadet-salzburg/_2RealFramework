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
#include <assert.h>

namespace _2Real
{
	Parameter::Parameter( std::shared_ptr< const CustomType > initializer ) :
		m_Data( new CustomType( *( initializer.get() ) ) ),			// clone
		m_DataBuffer( new CustomType( *( initializer.get() ) ) )	// clone
	{
#ifdef _DEBUG
		assert( m_DataBuffer.get() && m_Data.get() );
#endif
	}

	void Parameter::synchronize()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
#ifdef _DEBUG
		assert( m_DataBuffer.get() && m_Data.get() );
#endif
		m_Data = m_DataBuffer;
	}

	std::shared_ptr< const CustomType > Parameter::getData() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
#ifdef _DEBUG
		assert( m_Data.get() );
#endif
		return m_Data;
	}

	CustomType & Parameter::getWriteableData()
	{
#ifdef _DEBUG
		assert( m_DataBuffer.get() );
#endif
		return *( m_DataBuffer.get() );
	}

}