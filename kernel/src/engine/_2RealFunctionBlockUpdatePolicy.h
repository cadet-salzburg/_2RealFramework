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

#pragma once

#include "engine/_2RealInletPolicy.h"
#include "engine/_2RealAbstractUpdatePolicy.h"
#include "engine/_2RealInletBasedTrigger.h"
#include "engine/_2RealTimeBasedTrigger.h"
#include "helpers/_2RealPoco.h"

#include <map>

namespace _2Real
{

	class Inlet;
	class BasicInletIO;
	class OutletIO;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;

	class AbstractInletTriggerCtor
	{
	public:
		virtual AbstractInletBasedTrigger * createTrigger( BasicInletIO *inlet, AbstractStateManager *mgr, const bool isSingleWeight ) = 0;
		virtual ~AbstractInletTriggerCtor() {};
	};

	template< typename Condition >
	class InletTriggerCtor : public AbstractInletTriggerCtor
	{
	public:
		AbstractInletBasedTrigger * createTrigger( BasicInletIO *inlet, AbstractStateManager *mgr, const bool isSingleWeight )
		{
			return new InletBasedTrigger< Condition >( inlet, mgr, isSingleWeight );
		}
	};

	class FunctionBlockUpdatePolicy : public AbstractUpdatePolicy
	{

	public:

		FunctionBlockUpdatePolicy( EngineImpl *engine, AbstractUberBlock *owner );
		~FunctionBlockUpdatePolicy();

		// update policy has no concept of multiinlets
		void addInlet( BasicInletIO &io, Policy const& p );
		void removeInlet( BasicInletIO &io );

		void syncChanges();

		void setNewUpdateRate( const double rate );
		double getUpdateRate() const;

		void setInletPolicy( BasicInletIO &io, Policy const& p );

	private:

		struct InletPolicyInfo
		{
			InletPolicyInfo( AbstractInletTriggerCtor *c, AbstractInletBasedTrigger *t, const bool w ) :
				wasPolicyChanged( true ), isSingleWeight( w ),
				policy( Policy::ALWAYS ), ctor( c ), trigger( t ) {}

			~InletPolicyInfo() { delete ctor; delete trigger; }

			bool						wasPolicyChanged;
			bool						isSingleWeight;
			Policy						policy;
			AbstractInletTriggerCtor	*ctor;
			AbstractInletBasedTrigger	*trigger;
		};

		typedef std::map< BasicInletIO *, InletPolicyInfo * >						InletPolicyMap;
		typedef std::map< BasicInletIO *, InletPolicyInfo * >::iterator				InletPolicyIterator;
		typedef std::map< BasicInletIO *, InletPolicyInfo * >::const_iterator		InletPolicyConstIterator;

		template< typename T >
		friend class FunctionBlock;

		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockIOManager			*m_IOManager;

		mutable Poco::FastMutex			m_Access;
		bool							m_TimeChanged;
		bool							m_InletPoliciesChanged;
		TimeBasedTrigger				*m_TimeTrigger;
		long							m_UpdateTime;
		double							m_UpdateRate;
		InletPolicyMap					m_InletPolicies;

	};
}
