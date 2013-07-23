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
#include "engine/_2RealAbstractUberBlock.h"
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

	std::shared_ptr< CustomType > Parameter::getWriteableData()
	{
#ifdef _DEBUG
		assert( m_DataBuffer.get() );
#endif
		return m_DataBuffer;
	}

	InputParameter::InputParameter( AbstractUberBlock &owningBlock, std::string const& name, std::shared_ptr< const CustomType > initializer ) :
		NonCopyable< InputParameter >(),
		Identifiable< InputParameter >( owningBlock.getIds(), name ),
		Handleable< InputParameter, bundle::ParameterHandle >( *this ),
		m_OwningUberBlock( owningBlock ),
		m_Data( new CustomType( *( initializer.get() ) ) ),			// clone
		m_DataBuffer( new CustomType( *( initializer.get() ) ) ),	// clone
		m_LastData()
	{
		( void )( name );
	}

	bool InputParameter::hasChanged() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		return ( *m_Data.get() == *m_LastData.get() );
	}

	void InputParameter::synchronize()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_LastData = m_Data;
		m_Data = m_DataBuffer;
	}

	std::shared_ptr< const CustomType > InputParameter::getData() const
	{
		return m_Data;
	}

	std::shared_ptr< const CustomType >InputParameter::getDataThreadsafe() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		return m_Data;
	}

	void InputParameter::setData( std::shared_ptr< const CustomType > data )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_DataAccess );
		m_DataBuffer = data;
	}

}