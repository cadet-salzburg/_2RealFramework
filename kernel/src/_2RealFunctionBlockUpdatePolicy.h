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

#include "_2RealAbstractUpdatePolicy.h"
#include "_2RealUberBlockBasedTrigger.h"
#include "_2RealInletBasedTrigger.h"
#include "_2RealTimeBasedTrigger.h"

#include <map>
#include <string>
#include <memory>

#include "Poco/Mutex.h"

namespace _2Real
{

	class Inlet;
	class ParameterData;
	class FunctionBlock;
	class FunctionBlockIOManager;
	class FunctionBlockStateManager;

	class AbstractInletTriggerCreator
	{
	public:
		virtual AbstractInletBasedTrigger * createTrigger( Inlet &inlet, AbstractStateManager &mgr ) = 0;
	};

	template< typename Condition >
	class InletTriggerCreator : public AbstractInletTriggerCreator
	{
	public:
		AbstractInletBasedTrigger * createTrigger( Inlet &inlet, AbstractStateManager &mgr )
		{
			return new InletBasedTrigger< Condition >( inlet, mgr );
		}
	};

	class FunctionBlockUpdatePolicy : public AbstractUpdatePolicy
	{

	public:

		typedef std::shared_ptr< AbstractInletTriggerCreator >	InletTriggerCtor;

		FunctionBlockUpdatePolicy( FunctionBlock &owner );

		void addInlet( Inlet &inlet );

		void changePolicy();

		void setNewUpdateTime( const long time );
		void setNewInletDefaultPolicy( InletTriggerCtor &inletDefault );
		void setNewInletPolicy( Inlet &inlet, InletTriggerCtor &inletPolicy );

	private:

		typedef std::shared_ptr< AbstractTimeBasedTrigger >		TimeTriggerPtr;
		typedef std::shared_ptr< AbstractInletBasedTrigger >	InletTriggerPtr;

		typedef std::map< Inlet *, InletTriggerCtor >		InletPolicyMap;
		typedef std::map< Inlet *, InletTriggerPtr >		InletTriggerMap;

		friend class FunctionBlock;

		FunctionBlockStateManager		*m_StateManager;
		FunctionBlockIOManager			*m_IOManager;

		mutable Poco::FastMutex			m_Access;
		bool							m_WasChanged;	// true if user configured anything since the last update

		long							m_UpdateTime;
		TimeTriggerPtr					m_TimeTrigger;

		InletPolicyMap					m_InletPolicies;
		InletTriggerMap					m_InletTriggers;

		InletTriggerCtor				m_ValidDataCtor;
		InletTriggerCtor				m_NewDataCtor;

	};

}