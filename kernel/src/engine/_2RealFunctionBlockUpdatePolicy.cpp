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
#include "engine/_2RealInlet.h"
#include "engine/_2RealAbstractIOManager.h"
#include "engine/_2RealFunctionBlockStateManager.h"
#include "helpers/_2RealHelpers.h"

using std::string;
using std::make_pair;
using std::greater;

#include <assert.h>

namespace _2Real
{

	FunctionBlockUpdatePolicy::FunctionBlockUpdatePolicy( AbstractUberBlock &owner ) :
		AbstractUpdatePolicy( owner ),
		m_Engine( EngineImpl::instance() ),
		m_TimeChanged( false ),
		m_InletPoliciesChanged( false ),
		m_TimeTrigger( nullptr ),
		m_UpdateTime( -1 ),
		m_UpdateRate( 0.0 )
	{
	}

	FunctionBlockUpdatePolicy::~FunctionBlockUpdatePolicy()
	{
		safeDelete( m_TimeTrigger );

		for ( InletPolicyIterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); /**/ )
		{
			delete it->second;
			it = m_InletPolicies.erase( it );
		}
	}

	void FunctionBlockUpdatePolicy::addInlet( BasicInletIO &inletIO, InletPolicy const& policy )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
#ifdef _DEBUG
		if ( m_InletPolicies.find( &inletIO ) != m_InletPolicies.end() )
		{
			assert( NULL );
		}
#endif

		BasicInletBuffer *buffer = inletIO.getBufferPtr();

		AbstractInletTriggerCtor *c;
		AbstractInletBasedTrigger *t;
		bool s;

		if ( policy == InletPolicy::ALWAYS )
		{
			c = new InletTriggerCtor< ValidData >();
			t = c->createTrigger( *buffer, *m_StateManager, false );
			s = false;
		}
		else if ( policy == InletPolicy::OR_NEWER_DATA )
		{
			c = new InletTriggerCtor< NewerTimestamp >();
			t = c->createTrigger( *buffer, *m_StateManager, true );
			s = true;
		}
		else if ( policy == InletPolicy::AND_NEWER_DATA )
		{
			c = new InletTriggerCtor< NewerTimestamp >();
			t = c->createTrigger( *buffer, *m_StateManager, false );
			s = false;
		}

		InletPolicyInfo *p = new InletPolicyInfo( c, t, s );
		m_InletPolicies.insert( make_pair( &inletIO, p ) );
	}


	void FunctionBlockUpdatePolicy::removeInlet( BasicInletIO &inletIO )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		InletPolicyIterator it = m_InletPolicies.find( &inletIO );
#ifdef _DEBUG
		if ( it == m_InletPolicies.end() )
		{
			assert( NULL );
		}
#endif
		delete it->second;
		m_InletPolicies.erase( it );
	}

	void FunctionBlockUpdatePolicy::syncChanges()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		if ( ! ( m_TimeChanged || m_InletPoliciesChanged ) ) return;

		if ( m_TimeChanged )
		{
			safeDelete( m_TimeTrigger );
			if ( m_UpdateTime > 0 )
			{
				m_TimeTrigger = new TimeBasedTrigger( *m_StateManager, m_UpdateTime );
			}

			m_TimeChanged = false;
		}

		if ( m_InletPoliciesChanged )
		{
			for ( InletPolicyIterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); ++it )
			{
				InletPolicyInfo *policy = it->second;

				if ( policy->wasPolicyChanged )
				{
					BasicInletBuffer *buffer = it->first->getBufferPtr();
					AbstractInletTriggerCtor *ctor = it->second->ctor;
					AbstractInletBasedTrigger *currTrigger = it->second->trigger;
					AbstractInletBasedTrigger *newTrigger = ctor->createTrigger( *buffer, *m_StateManager, it->second->isSingleWeight );

					if ( currTrigger != nullptr )
					{
						*newTrigger = *currTrigger;		// copies the triggering data + timestamp
					}

					delete currTrigger;
					it->second->trigger = newTrigger;

					it->second->wasPolicyChanged = false;
				}
			}

			m_InletPoliciesChanged = false;
		}
	}

	void FunctionBlockUpdatePolicy::setNewUpdateRate( const double rate )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_TimeChanged = true;
		if ( rate <= 0. )
		{
			m_UpdateTime = -1;
			m_UpdateRate = 0.0;
		}

		double micros = 1000000/rate;
		m_UpdateTime = static_cast< long >(micros);
		m_UpdateRate = rate;
	}

	void FunctionBlockUpdatePolicy::setInletPolicy( BasicInletIO &io, InletPolicy const& p )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_InletPoliciesChanged = true;
		InletPolicyIterator it = m_InletPolicies.find( &io );
		if ( it != m_InletPolicies.end() )
		{
			it->second->wasPolicyChanged = true;
			delete it->second->ctor;

			if ( p == InletPolicy::ALWAYS )
			{
				it->second->ctor = new InletTriggerCtor< ValidData >();
				it->second->isSingleWeight = false;
			}
			else if ( p == InletPolicy::OR_NEWER_DATA )
			{
				it->second->ctor = new InletTriggerCtor< NewerTimestamp >();
				it->second->isSingleWeight = true;
			}
			else if ( p == InletPolicy::AND_NEWER_DATA )
			{
				it->second->ctor = new InletTriggerCtor< NewerTimestamp >();
				it->second->isSingleWeight = false;
			}
		}
		else
		{
#ifdef _DEBUG
			assert( NULL );
#endif
		}
	}

	double FunctionBlockUpdatePolicy::getUpdateRate() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		return m_UpdateRate;
	}

}
