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

#include "engine/_2RealAbstractUpdatePolicy.h"
#include "engine/_2RealInletBasedTrigger.h"
#include "engine/_2RealTimeBasedTrigger.h"
#include "helpers/_2RealPoco.h"

#include <map>

namespace _2Real
{

	class Inlet;
	class InletIO;
	class OutletIO;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;

	class AbstractInletTriggerCtor
	{
	public:
		virtual AbstractInletBasedTrigger * createTrigger( InletBuffer &buffer, AbstractStateManager &mgr, const bool isOr ) = 0;
		virtual ~AbstractInletTriggerCtor() {};
	};

	template< typename Condition >
	class InletTriggerCtor : public AbstractInletTriggerCtor
	{
	public:
		AbstractInletBasedTrigger * createTrigger( InletBuffer &buffer, AbstractStateManager &mgr, const bool isOr )
		{
			return new InletBasedTrigger< Condition >( buffer, mgr, isOr );
		}
	};

	class FunctionBlockUpdatePolicy : public AbstractUpdatePolicy
	{

	public:

		FunctionBlockUpdatePolicy( AbstractUberBlock &owner );
		~FunctionBlockUpdatePolicy();

		void addInlet( InletIO &io );
		void changePolicy();
		void setNewUpdateRate( const double rate );
		void setNewInletPolicy( InletIO &io, AbstractInletTriggerCtor *policy, const bool isOr, std::string const& typeName );
		const std::string getUpdatePolicyAsString( std::string const& inlet ) const;
		double getUpdateRate() const;

	private:

		struct InletPolicy
		{
			~InletPolicy();
			bool						wasChanged;
			bool						isOr;
			AbstractInletTriggerCtor	*ctor;
			AbstractInletBasedTrigger	*trigger;
			std::string					typeString;
		};

		typedef std::map< InletIO *, InletPolicy * >						InletPolicyMap;
		typedef std::map< InletIO *, InletPolicy * >::iterator				InletPolicyIterator;
		typedef std::map< InletIO *, InletPolicy * >::const_iterator		InletPolicyConstIterator;

		template< typename T >
		friend class FunctionBlock;

		EngineImpl						&m_Engine;
		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockIOManager			*m_IOManager;

		mutable Poco::FastMutex			m_Access;
		bool							m_TimeChanged;
		bool							m_InletsChanged;
		TimeBasedTrigger				*m_TimeTrigger;
		long							m_UpdateTime;
		double							m_UpdateRate;
		InletPolicyMap					m_InletPolicies;

	};
}
