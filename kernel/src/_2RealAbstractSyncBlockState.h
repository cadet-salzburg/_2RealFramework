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

#include <string>
#include <sstream>

namespace _2Real
{

	class SyncStates;
	class SyncUpdates;

	class AbstractSyncState
	{

	public:

		virtual ~AbstractSyncState() {}

		virtual void prepareForShutDown(SyncStates &states) const = 0;
		virtual void shutDown(SyncStates &states) const = 0;
		virtual void subBlocksFinished(SyncStates &states) const = 0;
		virtual void subBlocksReady(SyncStates &states) const = 0;

	};

	class SyncStateCreated : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStateCreated::prepareForShutDown(SyncStates &states) const
	{
		states.disableAll();
	}

	inline void SyncStateCreated::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateCreated::subBlocksFinished(SyncStates &states) const
	{
		states.resetAndEnableReadySet();
	}

	inline void SyncStateCreated::subBlocksReady(SyncStates &states) const
	{
		//this should actually never happen
	}

	class SyncStatePreShutDown : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStatePreShutDown::prepareForShutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStatePreShutDown::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStatePreShutDown::subBlocksFinished(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStatePreShutDown::subBlocksReady(SyncStates &states) const
	{
		//nothing
	}

	class SyncStateShutDown : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStateShutDown::prepareForShutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateShutDown::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateShutDown::subBlocksFinished(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateShutDown::subBlocksReady(SyncStates &states) const
	{
		//nothing
	}

	class SyncStateError : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStateError::prepareForShutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateError::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateError::subBlocksFinished(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateError::subBlocksReady(SyncStates &states) const
	{
		//nothing
	}

	class SyncStateWaiting : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStateWaiting::prepareForShutDown(SyncStates &states) const
	{
		states.disableAll();
	}

	inline void SyncStateWaiting::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateWaiting::subBlocksFinished(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateWaiting::subBlocksReady(SyncStates &states) const
	{
		//triggers.disableReadySet();
		states.resetAndEnableFinishedSet();
	}

	class SyncStateExecuting : public AbstractSyncState
	{
		void prepareForShutDown(SyncStates &states) const;
		void shutDown(SyncStates &states) const;
		void subBlocksFinished(SyncStates &states) const;
		void subBlocksReady(SyncStates &states) const;
	};

	inline void SyncStateExecuting::prepareForShutDown(SyncStates &states) const
	{
		states.disableAll();
	}

	inline void SyncStateExecuting::shutDown(SyncStates &states) const
	{
		//nothing
	}

	inline void SyncStateExecuting::subBlocksFinished(SyncStates &states) const
	{
		states.disableFinishedSet();
		states.resetAndEnableReadySet();
	}

	inline void SyncStateExecuting::subBlocksReady(SyncStates &states) const
	{
		//nothing
	}

}