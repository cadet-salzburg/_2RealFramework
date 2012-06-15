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

#include "_2RealDisabledStates.h"
#include "_2RealAbstractUberBlock.h"

#include <assert.h>

namespace _2Real
{

	SystemStates::SystemStates(AbstractBlock &owner) :
		AbstractStateManager(owner)
	{
	}

	void SystemStates::start()
	{
	}

	void SystemStates::setUp()
	{
	}

	Poco::Event & SystemStates::stop()
	{
		return m_StopEvent;
	}

	void SystemStates::prepareForShutDown()
	{
	}

	bool SystemStates::shutDown( const long timeout )
	{
		return true;
	}

	void SystemStates::setUpdatePolicy( UpdatePolicyImpl const& policy )
	{
	}

	void SystemStates::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
	}

	void SystemStates::tryTriggerTime(long &time)
	{
	}

	void SystemStates::tryTriggerSubBlock( AbstractStateManager &sub, const BlockMessage msg )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UpdateAccess );
		std::map< std::string, unsigned long >::iterator it = m_BlockUpdates.find( sub.getName() );
		if ( it != m_BlockUpdates.end() && it->second > 0 )
		{
			it->second -= 1;	// consume an allowed update
			sub.tryTriggerUberBlock( *this, BLOCK_OK );
		}
		else if ( it != m_BlockUpdates.end() && it->second == 0 )
		{
			sub.stop();
		}
		else
		{
#ifdef _DEBUG
			assert( NULL );
#endif
		}
	}

	void SystemStates::tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg)
	{
	}

	void SystemStates::subBlockAdded( AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage msg )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UpdateAccess );
		m_BlockUpdates[ subBlock.getName() ] = static_cast< unsigned long >( 0 );
	}

	void SystemStates::subBlockRemoved( AbstractBlock &subBlock )
	{
	}

	void SystemStates::setAllowedUpdates( AbstractBlock &block, const unsigned long updates )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UpdateAccess );
		m_BlockUpdates[ block.getName() ] = updates;
	}

	void SystemStates::uberBlockAdded( AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage msg )
	{
	}

	void SystemStates::uberBlockRemoved( AbstractBlock &uberBlock )
	{
	}

	void SystemStates::inletAdded( Inlet &inlet, AbstractInletBasedTrigger &trigger )
	{
	}

	void SystemStates::inletRemoved( Inlet &inlet )
	{
	}

}