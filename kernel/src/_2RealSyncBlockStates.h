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

#include "_2RealException.h"

#include "Poco/Mutex.h"

namespace _2Real
{

	class AbstractSyncState;
	class Logger;

	class SyncStates : public AbstractStateManager
	{

	public:

		SyncStates(AbstractBlock &owner);
		~SyncStates();

		void		clear();
		void		setUp();
		void		prepareForShutDown();
		const bool	shutDown();

		//called by sync triggers
		void		waitForReadySet();
		void		waitForFinishedSet();

		void resetAndEnableReadySet();
		void resetAndEnableFinishedSet();
		void disableAll();
		void disableReadySet();
		void disableFinishedSet();
		void notifyFinishedSet();
		void notifyReadySet();

		void tryTriggerTime(long &time);
		void tryTriggerInlet(const void *inlet, std::pair< long, long > &times);
		void tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage msg);
		void tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg);

		void subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg);
		void subBlockRemoved(AbstractBlock &subBlock);
		void uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg);
		void uberBlockRemoved(AbstractBlock &uberBlock);
		void inletAdded(Inlet &slot, AbstractInletBasedTrigger &trigger);
		void inletRemoved(Inlet &slot);

	private:

		friend class SyncBlock;

		void handleStateChangeException(Exception &e);

		void evaluateReadyTriggers();
		void evaluateFinishedTriggers();
		const bool areReadyTriggersEnabled() const;
		const bool areFinishedTriggersEnabled() const;

		mutable Poco::FastMutex									m_ReadyAccess;
		mutable Poco::FastMutex									m_FinishedAccess;
		TriggerMap												m_ReadySet;
		TriggerMap												m_FinishedSet;

		mutable Poco::FastMutex									m_EnabledAccess;
		bool													m_ReadyTriggersEnabled;
		bool													m_FinishedTriggersEnabled;

		mutable Poco::FastMutex									m_StateAccess;
		AbstractSyncState										*m_CurrentState;
		Logger													&m_Logger;

	};

}