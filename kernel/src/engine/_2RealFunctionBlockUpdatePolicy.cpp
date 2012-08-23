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

	FunctionBlockUpdatePolicy::InletPolicy::~InletPolicy()
	{
		delete ctor;
		delete trigger;
	}

	FunctionBlockUpdatePolicy::FunctionBlockUpdatePolicy( AbstractUberBlock &owner ) :
		AbstractUpdatePolicy( owner ),
		m_Engine( EngineImpl::instance() ),
		m_TimeChanged( false ),
		m_InletsChanged( false ),
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

	void FunctionBlockUpdatePolicy::addInlet( InletIO &inletIO )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
#ifdef _DEBUG
		if ( m_InletPolicies.find( &inletIO ) != m_InletPolicies.end() )
		{
			assert( NULL );
		}
#endif

		m_InletsChanged = true;
		InletPolicy *p = new InletPolicy();
		p->ctor = new InletTriggerCtor< ValidData >();
		p->isOr = false;
		p->trigger = nullptr;
		p->wasChanged = true;
		p->typeString = "valid_data";
		m_InletPolicies.insert( make_pair( &inletIO, p ) );
	}

	void FunctionBlockUpdatePolicy::changePolicy()
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );

		if ( ! ( m_TimeChanged || m_InletsChanged ) ) return;

		if ( m_TimeChanged )
		{
			safeDelete( m_TimeTrigger );
			if ( m_UpdateTime > 0 )
			{
				m_TimeTrigger = new TimeBasedTrigger( *m_StateManager, m_UpdateTime );
			}

			m_TimeChanged = false;
		}

		if ( m_InletsChanged )
		{
			for ( InletPolicyIterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); ++it )
			{
				if ( it->second->wasChanged )
				{
					InletBuffer *buffer = it->first->m_Buffer;
					AbstractInletTriggerCtor *ctor = it->second->ctor;
					AbstractInletBasedTrigger *currTrigger = it->second->trigger;
					AbstractInletBasedTrigger *newTrigger = ctor->createTrigger( *buffer, *m_StateManager, it->second->isOr );

					if ( currTrigger != nullptr )
					{
						*newTrigger = *currTrigger;		// copies the triggering data + timestamp
					}

					delete currTrigger;
					it->second->trigger = newTrigger;

					it->second->wasChanged = false;
				}
			}
			m_InletsChanged = false;
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

	void FunctionBlockUpdatePolicy::setNewInletPolicy( InletIO &io, AbstractInletTriggerCtor *inletPolicy, const bool isOr, std::string const& typeName )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		m_InletsChanged = true;
		InletPolicyIterator it = m_InletPolicies.find( &io );
		if ( it != m_InletPolicies.end() )
		{
			it->second->wasChanged = true;
			delete it->second->ctor;
			it->second->ctor = inletPolicy;
			it->second->isOr = isOr;
			it->second->typeString = typeName;
		}
		else
		{
#ifdef _DEBUG
			assert( NULL );
#endif
			delete inletPolicy;
		}
	}

	const std::string FunctionBlockUpdatePolicy::getUpdatePolicyAsString( std::string const& inlet ) const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		for ( InletPolicyConstIterator it = m_InletPolicies.begin(); it != m_InletPolicies.end(); ++it )
		{
			if ( it->first->m_Inlet->getName() == inlet )
			{
				return it->second->typeString;
			}
		}
#ifdef _DEBUG
		assert( NULL );
#endif
			return "";
	}

	double FunctionBlockUpdatePolicy::getUpdateRate() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_Access );
		return m_UpdateRate;
	}

}
