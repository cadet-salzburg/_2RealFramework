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
#include "helpers/_2RealHelpersInternal.h"
#include "helpers/_2RealPoco.h"

namespace _2Real
{
	namespace bundle
	{
		class Block;
	}

	class AbstractFunctionBlockState;
	class AbstractUpdateTrigger;
	class UberBlockBasedTrigger;
	class ServiceUpdates;
	class FunctionBlockIOManager;
	class ThreadPool;
	class Logger;
	class System;
	class Block;
	class Exception;
	class FunctionBlock;
	class FunctionBlockUpdatePolicy;

	class FunctionBlockStateManager : public AbstractStateManager
	{

	public:

		FunctionBlockStateManager( FunctionBlock &owner );
		~FunctionBlockStateManager();

		void setUp();							// system
		void start();							// system
		Poco::Event & stop();					// system
		void beginUpdate();						// threadpool
		void finishUpdate();					// threadpool
		void prepareForShutDown();				// system
		bool shutDown( const long timeout );	// system
		void updateFunctionBlock();				// threadpool

		void tryTrigger( AbstractUpdateTrigger &trigger );
		void tryTriggerUberBlock( UberBlockBasedTrigger &trigger );

		void addTrigger( AbstractUpdateTrigger &trigger );
		void removeTrigger( AbstractUpdateTrigger &trigger );
		void addUberBlockTrigger( UberBlockBasedTrigger &trigger );
		void removeUberBlockTrigger( UberBlockBasedTrigger &trigger );

	private:

		friend class FunctionBlock;		//needs to set a few things

		void handleStateChangeException(Exception &e);
		void triggersAreOk();
		void uberBlocksAreOk();
		void disableAllTriggers();
		void disableTriggers();
		void disableUberBlockTriggers();
		void resetAllTriggers();
		void resetTriggers();
		void resetUberBlockTriggers();
		void enableAllTriggers();
		void enableTriggers();
		void enableUberBlockTriggers();
		bool areTriggersEnabled() const;
		bool areUberBlockTriggersEnabled() const;
		void evaluateTriggers();
		void evaluateUberBlockTriggers();

		ThreadPool										&m_Threads;
		Logger											&m_Logger;

		FunctionBlockIOManager							*m_IOManager;
		FunctionBlockUpdatePolicy						*m_UpdatePolicy;
		bundle::Block									*m_FunctionBlock;

		mutable Poco::FastMutex							m_TriggerAccess;
		AbstractStateManager::TriggerList				m_Triggers;

		mutable Poco::FastMutex							m_UberBlockTriggerAccess;
		AbstractStateManager::UberBlockTriggerList		m_UberBlockTriggers;

		mutable Poco::FastMutex							m_EnabledAccess;
		bool											m_UberBlockTriggersEnabled;
		bool											m_TriggersEnabled;

		mutable Poco::FastMutex							m_StateAccess;
		AbstractFunctionBlockState						*m_CurrentState;

		// the three types of user interactions that might occur during an update cycle
		// ( ok, in theory there's also start - but that's pretty irrelevant )
		SafeBool										m_FlaggedForSetUp;
		SafeBool										m_FlaggedForStop;

	};

}