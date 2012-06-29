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

#include "_2RealUpdatePolicy.h"
#include "_2RealEngineImpl.h"
#include "_2RealTimer.h"
#include "_2RealInletBuffer.h"
#include "_2RealAbstractStateManager.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealParameterData.h"

using std::string;
using std::make_pair;
using std::greater;
using std::shared_ptr;

#include <iostream>

namespace _2Real
{

	UpdatePolicy::UpdatePolicy( AbstractUberBlock &owner, AbstractStateManager &stateMgr, AbstractIOManager &ioMgr ) :
		m_Owner( owner ),
		m_WasChanged( false ),
		m_UpdateTime( -1 ),
		m_IOManager( &ioMgr ),
		m_StateManager( &stateMgr )
	{
	}

	void UpdatePolicy::addInlet( string const& name )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		m_InletPolicies.insert( make_pair( name, InletTriggerCtor( new InletTriggerCreator< ValidData >() ) ) );
		m_InletTriggers.insert( make_pair( name, InletTriggerPtr() ) );
	}

	void UpdatePolicy::changePolicy()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		if ( !m_WasChanged ) return;

		// this function is to be called by the state manager itself, after an update is finished but before the inlet buffers update themselves

		if ( m_TimeTrigger.get() != nullptr )
		{
			EngineImpl::instance().getTimer().unregisterFromTimerSignal( *m_TimeTrigger.get() );
			m_StateManager->removeTrigger( *m_TimeTrigger.get() );
		}

		if ( m_UpdateTime > 0 )
		{
			m_TimeTrigger.reset( new TimeBasedTrigger< std::greater< long > >( *m_StateManager, m_UpdateTime ) );
			m_StateManager->addTrigger( *m_TimeTrigger.get() );
			EngineImpl::instance().getTimer().registerToTimerSignal( *m_TimeTrigger.get() );
		}
		else
		{
			m_TimeTrigger.reset();
			//m_StateManager->addTrigger( *m_TimeTrigger.get() );
			//EngineImpl::instance().getTimer().registerToTimerSignal( *m_TimeTrigger.get() );
		}

		for ( InletTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			string name = it->first;
			InletTriggerPtr &triggerPtr = it->second;

			InletTriggerCtor &ctor = m_InletPolicies[ name ];
			Inlet &inlet = m_IOManager->getInlet( name );

			AbstractInletBasedTrigger *newTrigger = ctor->createTrigger( inlet, *m_StateManager );

			if ( triggerPtr.get() != nullptr )
			{
				*newTrigger = *triggerPtr.get();		// copies the triggering data
			}

			triggerPtr.reset( newTrigger );			// deletes old trigger -> unregisters itself
		}

		m_WasChanged = false;
	}

	void UpdatePolicy::setNewUpdateTime( const long time )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		m_UpdateTime = time;
	}

	void UpdatePolicy::setNewInletDefaultPolicy( InletTriggerCtor &inletDefault )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		for ( InletPolicyMap::iterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); ++it )
		{
			it->second = inletDefault;
		}
	}

	void UpdatePolicy::setNewInletPolicy( string const& name, InletTriggerCtor &inletPolicy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		InletPolicyMap::iterator it = m_InletPolicies.find( name );
		if ( it != m_InletPolicies.end() )
		{
			it->second = inletPolicy;
		}
	}

}