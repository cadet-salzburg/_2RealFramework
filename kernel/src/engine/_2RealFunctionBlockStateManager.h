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

#include "engine/_2RealAbstractStateManager.h"
#include "helpers/_2RealSynchronizedBool.h"
#include "helpers/_2RealPoco.h"

namespace _2Real
{
	namespace bundle
	{
		class Block;
	}

	class ThreadPool;
	class Logger;
	class Exception;
	class FunctionBlockUpdatePolicy;
	class AbstractFunctionBlockState;
	class FunctionBlockIOManager;

	class FunctionBlockStateManager : public AbstractStateManager
	{

	public:

		FunctionBlockStateManager( AbstractUberBlock &owner );
		~FunctionBlockStateManager();

		void setUp();							// system
		void start();							// system
		Poco::Event & stop();					// system
		void beginUpdate();						// threadpool
		void finishUpdate();					// threadpool
		void prepareForShutDown();				// system
		bool shutDown( const long timeout );	// system
		void updateFunctionBlock();				// threadpool

		void tryTriggerInlet( AbstractInletBasedTrigger &trigger );
		void tryTriggerTime( AbstractTimeBasedTrigger &trigger );

		void addTrigger( AbstractInletBasedTrigger &trigger );
		void removeTrigger( AbstractInletBasedTrigger &trigger );
		void addTrigger( AbstractTimeBasedTrigger &trigger );
		void removeTrigger( AbstractTimeBasedTrigger &trigger );

	private:

		template< typename T >
		friend class FunctionBlock;

		void handleStateChangeException( Exception &e );
		void triggersAreOk();
		void uberBlocksAreOk();
		void singleStep();
		void resetTriggers();

		ThreadPool							&m_Threads;
		Logger								&m_Logger;
		FunctionBlockIOManager				*m_IOManager;
		FunctionBlockUpdatePolicy			*m_UpdatePolicy;
		bundle::Block						*m_FunctionBlock;

		mutable Poco::FastMutex				m_TriggerAccess;
		InletTriggers						m_InletTriggers;
		AbstractTimeBasedTrigger			*m_TimeTrigger;

		mutable Poco::FastMutex				m_StateAccess;
		AbstractFunctionBlockState			*m_CurrentState;

		mutable Poco::FastMutex				m_EnabledAccess;
		bool								m_IsTriggeringEnabled;

		SafeBool							m_IsFlaggedForSetup;
		SafeBool							m_IsFlaggedForStop;
		SafeBool							m_IsFlaggedForShutdown;
		SafeBool							m_IsSingleStepping;

	};

}