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

#include "_2RealSystemBlockStates.h"
#include "_2RealAbstractUberBlock.h"

#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;

namespace _2Real
{

	SystemBlockStateManager::SystemBlockStateManager( AbstractUberBlock &owner ) :
		AbstractStateManager( owner )
	{
	}

	SystemBlockStateManager::~SystemBlockStateManager()
	{
		for ( UberBlockBasedTriggerMap::iterator it = m_SubBlockTriggers.begin(); it != m_SubBlockTriggers.end(); ++it )
		{
			delete it->second;
		}
	}

	void SystemBlockStateManager::start()
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::setUp()
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	Poco::Event & SystemBlockStateManager::stop()
	{
#ifdef _DEBUG
		assert( NULL );
#endif
		return m_StopEvent;
	}

	void SystemBlockStateManager::prepareForShutDown()
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	bool SystemBlockStateManager::shutDown( const long timeout )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
		return true;
	}

	void SystemBlockStateManager::setUpdatePolicy( UpdatePolicyImpl const& policy )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::tryTriggerTime(long &time)
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::tryTriggerSubBlock( const unsigned int id, const BlockMessage msg )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		UberBlockBasedTriggerMap::iterator it = m_SubBlockTriggers.find( id );
		if ( it != m_SubBlockTriggers.end() )
		{
			it->second->tryTriggerOther( BLOCK_OK );
		}
		else
		{
#ifdef _DEBUG
			assert( NULL );
#endif
		}
	}

	void SystemBlockStateManager::tryTriggerSuperBlock( const unsigned int id, const BlockMessage msg )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::addTriggerForSubBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_TriggerAccess );
		UberBlockBasedTriggerMap::iterator it = m_SubBlockTriggers.find( id );
		if ( it != m_SubBlockTriggers.end() )
		{
			delete it->second;
		}

		m_SubBlockTriggers[ id ] = &trigger;
	}

	void SystemBlockStateManager::removeTriggerForSubBlock( const unsigned int id )
	{
	}

	void SystemBlockStateManager::addTriggerForSuperBlock( const unsigned int id, AbstractUberBlockBasedTrigger &trigger )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::removeTriggerForSuperBlock( const unsigned int id )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

}