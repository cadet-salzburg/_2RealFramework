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

#include "_2RealSystemBlockStateManager.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealUberBlockBasedTrigger.h"

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

	void SystemBlockStateManager::tryTrigger( AbstractUpdateTrigger &trigger )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::tryTriggerUberBlock( UberBlockBasedTrigger &trigger )
	{
		trigger.reset();
		trigger.tryTriggerOther( BLOCK_OK );		// system always allows an update
	}

	void SystemBlockStateManager::addUberBlockTrigger( UberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		for ( UberBlockTriggerList::iterator it = m_SubBlockTriggers.begin(); it != m_SubBlockTriggers.end(); ++it )
		{
			if ( *it == &trigger )
			{
				m_SubBlockTriggers.erase( it );
				break;
			}
		}
	}

	void SystemBlockStateManager::removeUberBlockTrigger( UberBlockBasedTrigger &trigger )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_SubBlockTriggers.push_back( &trigger );
	}

	void SystemBlockStateManager::addTrigger( AbstractUpdateTrigger &trigger )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

	void SystemBlockStateManager::removeTrigger( AbstractUpdateTrigger &trigger )
	{
#ifdef _DEBUG
		assert( NULL );
#endif
	}

}