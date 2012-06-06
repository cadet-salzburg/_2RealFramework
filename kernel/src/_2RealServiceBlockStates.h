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

#include "Poco/Event.h"
#include "Poco/Mutex.h"

namespace _2Real
{

	class AbstractServiceState;
	class ServiceUpdates;
	class ServiceIO;
	class ThreadPool;
	class Logger;
	class SystemImpl;

	class ServiceStates : public AbstractStateManager
	{

	public:

		ServiceStates(AbstractBlock &owner);
		~ServiceStates();

		void clear();
		void setUp();
		void triggersReady();
		void beginUpdate();
		void beginExecution();
		void finishExecution();
		void prepareForShutDown();
		const bool shutDown();
		void abort();

		void disableAllTriggers();

		void setUpService();
		void executeService();
		void shutDownService();
		void flagForSetUp();
		void flagForShutDown();
		void flagStopped();

		void tryTriggerTime(long &time);
		void tryTriggerInlet(const void *inlet, std::pair< long, long > &times);
		void tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage msg);
		void tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg);

		void subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg);
		void subBlockRemoved(AbstractBlock &subBlock);
		void uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg);
		void uberBlockRemoved(AbstractBlock &uberBlock);

		const bool conditionalSetUp();

	private:

		friend class ServiceBlock;

		const bool areTriggersEnabled() const;
		const bool areUberTriggersEnabled() const;
		void resetAndEnableTriggers();
		void resetAndEnableUberTriggers();
		void disableTriggers();
		void disableUberTriggers();
		void initFrom(UpdatePolicyImpl const& policy);
		void evaluateTriggers();
		void evaluateUberTriggers();
		bool hasUberTriggers();
		void notifyUberTriggers( const BlockMessage msg );

		void handleStateChangeException(Exception &e);

		mutable Poco::FastMutex									m_TriggerAccess;
		AbstractTimeBasedTrigger								*m_Time;
		std::map< std::string, AbstractInletBasedTrigger * >	m_Inlets;

		mutable Poco::FastMutex									m_UberTriggerAccess;
		TriggerMap												m_UberTriggers;

		mutable Poco::FastMutex									m_EnabledAccess;
		bool													m_UberTriggersEnabled;
		bool													m_TriggersEnabled;

		mutable Poco::FastMutex									m_StateAccess;
		AbstractServiceState									*m_CurrentState;

		bool													m_FlaggedForSetUp;
		bool													m_FlaggedForShutDown;
		bool													m_FlaggedForAbort;
		Poco::Event												m_StopEvent;

		ServiceIO												*m_IO;
		SystemImpl												*m_System;
		ThreadPool												&m_Threads;
		Logger													&m_Logger;

	};

}