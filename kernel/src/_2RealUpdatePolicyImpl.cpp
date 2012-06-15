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

#include "_2RealUpdatePolicyImpl.h"
#include "_2RealHelpersInternal.h"

#include <map>

namespace _2Real
{

	UpdatePolicyImpl::UpdatePolicyImpl() :
		m_Time( -1 )
		//m_InletDefault( nullptr ),
		//m_BlockDefault( nullptr )
	{
	}

	UpdatePolicyImpl::~UpdatePolicyImpl()
	{
	}

	void UpdatePolicyImpl::clear()
	{
		//for (std::map< std::string, AbstractInletDefault * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
		//{
		//	delete it->second;
		//	it = m_Inlets.erase(it);
		//}

		//for (std::map< std::string, AbstractBlockDefault * >::iterator it = m_Blocks.begin(); it != m_Blocks.end(); /**/)
		//{
		//	delete it->second;
		//	it = m_Blocks.erase(it);
		//}

		//if ( m_InletDefault != nullptr )
		//{
		//	delete m_InletDefault;
		//	m_InletDefault = nullptr;
		//}

		//if ( m_BlockDefault != nullptr )
		//{
		//	delete m_BlockDefault;
		//	m_BlockDefault = nullptr;
		//}

		m_Time = -1;
	}

	void UpdatePolicyImpl::setInletDefault( UpdatePolicyImpl::InletDefaultPtr &inletDefault )
	{
		//if ( m_InletDefault != nullptr )
		//{
		//	delete m_InletDefault;
		//}

		m_InletDefault = inletDefault;
	}

	void UpdatePolicyImpl::setBlockDefault( UpdatePolicyImpl::BlockDefaultPtr &blockDefault )
	{
		//if ( m_BlockDefault != nullptr )
		//{
		//	delete m_BlockDefault;
		//}

		m_BlockDefault = blockDefault;
	}

	void UpdatePolicyImpl::addTimeBasedTrigger( const long time )
	{
		m_Time = time;
	}

	void UpdatePolicyImpl::addInletBasedTrigger( std::string const& inletName, UpdatePolicyImpl::InletDefaultPtr &trigger )
	{
		//std::map< std::string, AbstractInletDefault * >::iterator it = m_Inlets.find( inletName );
		//if ( it != m_Inlets.end() )
		//{
		//	delete it->second;
		//	m_Inlets.erase( it );
		//}
		//m_Inlets.insert( std::make_pair( inletName, trigger ) );

		m_Inlets[ inletName ] = trigger;
	}

	void UpdatePolicyImpl::addBlockBasedTrigger( std::string const& blockName, UpdatePolicyImpl::BlockDefaultPtr &trigger )
	{
		//std::map< std::string, AbstractBlockDefault * >::iterator it = m_Blocks.find( blockName );
		//if ( it != m_Blocks.end() )
		//{
		//	delete it->second;
		//	m_Blocks.erase( it );
		//}

		//m_Blocks.insert( std::make_pair( blockName, trigger ) );

		m_Blocks[ blockName ] = trigger;
	}

	bool UpdatePolicyImpl::hasTimeBasedTrigger() const
	{
		return m_Time > 0;
	}

	AbstractTimeBasedTrigger * UpdatePolicyImpl::getTimeBasedTrigger() const
	{
		return new TimeBasedTrigger< std::greater_equal< long > >( m_Time );
	}

	bool UpdatePolicyImpl::hasTriggerForInlet( std::string const& inletName ) const
	{
		return ( ( m_Inlets.find( inletName ) != m_Inlets.end() ) || m_InletDefault != nullptr );
	}

	AbstractInletBasedTrigger * UpdatePolicyImpl::getTriggerForInlet( std::string const& inletName ) const
	{
		std::map< std::string, InletDefaultPtr >::const_iterator it = m_Inlets.find( inletName );

		if ( it != m_Inlets.end() )
		{
			return it->second->createTrigger( inletName );
		}
		else if ( m_InletDefault != nullptr )
		{
			return m_InletDefault->createTrigger( inletName );
		}
		else
		{
			return nullptr;
		}
	}

	AbstractBlockBasedTrigger * UpdatePolicyImpl::getBlockBasedTrigger( std::string const& blockName ) const
	{
		std::map< std::string, BlockDefaultPtr >::const_iterator it = m_Blocks.find( blockName );

		if ( it != m_Blocks.end() )
		{
			return it->second->createTrigger( blockName );
		}
		else if ( m_BlockDefault != nullptr )
		{
			return m_BlockDefault->createTrigger( blockName );
		}
		else
		{
			return nullptr;
		}
	}

}