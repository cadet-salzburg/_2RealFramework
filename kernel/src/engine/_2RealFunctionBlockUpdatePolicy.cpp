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

#include "engine/_2RealFunctionBlockUpdatePolicy.h"
#include "engine/_2RealEngineImpl.h"
#include "engine/_2RealInletBuffer.h"
#include "engine/_2RealFunctionBlock.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "engine/_2RealFunctionBlockIOManager.h"
#include "engine/_2RealParameterData.h"

using std::string;
using std::make_pair;
using std::greater;
using std::shared_ptr;

#include <iostream>

namespace _2Real
{

	FunctionBlockUpdatePolicy::FunctionBlockUpdatePolicy( FunctionBlock &owner ) :
		AbstractUpdatePolicy( owner ),
		m_WasChanged( false ),
		m_UpdateTime( -1 ),
		m_TimeTrigger( nullptr )
	{
	}

	FunctionBlockUpdatePolicy::~FunctionBlockUpdatePolicy()
	{
		if ( m_TimeTrigger != nullptr )
		{
			delete m_TimeTrigger;
			m_TimeTrigger = nullptr;
		}
	}

	void FunctionBlockUpdatePolicy::addInlet( Inlet &inlet )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		m_WasChanged = true;
		m_InletPolicies.insert( make_pair( &inlet, InletTriggerCtor( new InletTriggerCreator< ValidData, false >() ) ) );
		m_InletTriggers.insert( make_pair( &inlet, InletTriggerPtr() ) );
	}

	void FunctionBlockUpdatePolicy::changePolicy()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		if ( !m_WasChanged ) return;

		if ( m_TimeTrigger != nullptr )
		{
			delete m_TimeTrigger;
			m_TimeTrigger = nullptr;
		}

		if ( m_UpdateTime > 0 )
		{
			m_TimeTrigger = new TimeBasedTrigger< std::greater< long > >( *m_StateManager, m_UpdateTime );
		}

		for ( InletTriggerMap::iterator it = m_InletTriggers.begin(); it != m_InletTriggers.end(); ++it )
		{
			Inlet *inlet = it->first;
			InletTriggerPtr &triggerPtr = it->second;
			InletTriggerCtor &ctor = m_InletPolicies[ inlet ];

			AbstractInletBasedTrigger *newTrigger = ctor->createTrigger( *inlet, *m_StateManager );

			if ( triggerPtr.get() != nullptr )
			{
				*newTrigger = *triggerPtr.get();		// copies the triggering data
			}

			triggerPtr.reset( newTrigger );			// deletes old trigger -> unregisters itself
		}

		m_WasChanged = false;
	}

	void FunctionBlockUpdatePolicy::setNewUpdateTime( const long time )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		m_UpdateTime = time;
	}

	void FunctionBlockUpdatePolicy::setNewInletDefaultPolicy( InletTriggerCtor &inletDefault )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		for ( InletPolicyMap::iterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); ++it )
		{
			it->second = inletDefault;
		}
	}

	void FunctionBlockUpdatePolicy::setNewInletPolicy( Inlet &inlet, InletTriggerCtor &inletPolicy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_WasChanged = true;
		InletPolicyMap::iterator it = m_InletPolicies.find( &inlet );
		if ( it != m_InletPolicies.end() )
		{
			it->second = inletPolicy;
		}
	}

}