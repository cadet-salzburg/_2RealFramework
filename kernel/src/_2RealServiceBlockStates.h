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

#include "_2RealAbstractStateManager.h"
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealHelpersInternal.h"

#include "Poco/Event.h"
#include "Poco/Mutex.h"

namespace _2Real
{

	class AbstractFunctionBlockState;
	class ServiceUpdates;
	class ServiceIO;
	class ThreadPool;
	class Logger;
	class SystemImpl;
	class Block;

	class FunctionBlockStateManager : public AbstractStateManager
	{

	public:

		FunctionBlockStateManager( AbstractBlock &owner );
		~FunctionBlockStateManager();

		void setUp();							// system
		void start();							// system
		Poco::Event & stop();					// system
		void beginUpdate();						// threadpool
		void finishUpdate();					// threadpool
		void prepareForShutDown();				// system
		bool shutDown( const long timeout );	// system
		void updateFunctionBlock();				// threadpool
		void setUpdatePolicy( UpdatePolicyImpl const& policy );

		void tryTriggerTime( long &time );
		void tryTriggerInlet( const void *inlet, std::pair< long, long > &times );
		void tryTriggerSubBlock( AbstractStateManager &sub, const BlockMessage msg ) {}
		void tryTriggerUberBlock( AbstractStateManager &uber, const BlockMessage msg );

		void subBlockAdded( AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg ) {}
		void subBlockRemoved( AbstractBlock &subBlock) {}
		void uberBlockAdded( AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg );
		void uberBlockRemoved( AbstractBlock &uberBlock);

	private:

		friend class FunctionBlock;		//needs to set a few things

		void changePolicy();
		void handleStateChangeException(Exception &e);
		void triggersAreOk();
		void uberBlocksAreOk();
		void disableAllTriggers();
		void disableTriggers();
		void disableUberBlockTriggers();
		void resetAllTriggers();
		void resetTriggers();
		void resetUberBlockTriggers( const bool notify );
		void enableAllTriggers();
		void enableTriggers();
		void enableUberBlockTriggers();
		bool areTriggersEnabled() const;
		bool areUberBlockTriggersEnabled() const;
		void evaluateTriggers();
		void evaluateUberBlockTriggers();

		ThreadPool						&m_Threads;
		Logger							&m_Logger;
		ServiceIO						*m_IO;
		SystemImpl						*m_System;
		Block							*m_FunctionBlock;

		mutable Poco::FastMutex			m_TriggerAccess;
		AbstractTimeBasedTrigger		*m_TimeTrigger;
		InletTriggerMap					m_InletTriggers;

		mutable Poco::FastMutex			m_UberTriggerAccess;
		BlockTriggerMap					m_UberTriggers;

		mutable Poco::FastMutex			m_EnabledAccess;
		bool							m_UberTriggersEnabled;
		bool							m_TriggersEnabled;

		mutable Poco::FastMutex			m_StateAccess;
		AbstractFunctionBlockState		*m_CurrentState;

		// the three types of user interactions that might occur during an update cycle
		// ( ok, in theory there's also start - but that's pretty irrelevant )
		SafeBool						m_FlaggedForSetUp;
		SafeBool						m_FlaggedForStop;
		SafeBool						m_FlaggedForPolicyChange;

		// needed for policy change
		Poco::FastMutex					m_PolicyAccess;
		UpdatePolicyImpl				const* m_NewPolicy;
		UpdatePolicyImpl				const* m_CurrentPolicy;

	};

}