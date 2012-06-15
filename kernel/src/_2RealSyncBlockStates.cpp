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

#include "_2RealSyncBlockStates.h"
#include "_2RealAbstractUberBlock.h"
#include "_2RealAbstractSyncBlockState.h"
#include "_2RealEngineImpl.h"
#include "_2RealLogger.h"

namespace _2Real
{

	SyncStates::SyncStates(AbstractBlock &owner) :
		AbstractStateManager(owner),
		m_CurrentState(new SyncStateCreated),
		m_Logger( EngineImpl::instance().getLogger() )
	{
	}

	SyncStates::~SyncStates()
	{
		delete m_CurrentState;

		for ( BlockTriggerMap::iterator it = m_ReadySet.begin(); it != m_ReadySet.end(); /**/ )
		{
			delete it->second.first;
			it = m_ReadySet.erase(it);
		}

		for ( BlockTriggerMap::iterator it = m_FinishedSet.begin(); it != m_FinishedSet.end(); /**/ )
		{
			delete it->second.first;
			it = m_FinishedSet.erase(it);
		}
	}

	void SyncStates::setUpdatePolicy( UpdatePolicyImpl const& policy )
	{
	}

	void SyncStates::setUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->subBlocksFinished(*this);
			delete m_CurrentState;
			m_CurrentState = new SyncStateWaiting();
			//m_Logger.addLine( std::string( getName() + " new sync state: waiting (system called)" ) );
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void SyncStates::start()
	{
	}

	Poco::Event & SyncStates::stop()
	{
		return m_StopEvent;
	}

	void SyncStates::prepareForShutDown()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->prepareForShutDown(*this);
			delete m_CurrentState;
			m_CurrentState = new SyncStatePreShutDown();
			//m_Logger.addLine( std::string( getName() + " new sync state: prepared to shut down" ) );
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	bool SyncStates::shutDown( const long timeout )
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->shutDown( *this );
			delete m_CurrentState;
			m_CurrentState = new SyncStateShutDown();
			//m_Logger.addLine( std::string( getName() + " new sync state: shut down" ) );
			return true;
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
			return false;
		}
	}

	void SyncStates::waitForReadySet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->subBlocksFinished(*this);
			delete m_CurrentState;
			m_CurrentState = new SyncStateWaiting();
			//m_Logger.addLine( std::string( getName() + " new sync state: waiting (finished exec)" ) );
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void SyncStates::waitForFinishedSet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->subBlocksReady(*this);
			delete m_CurrentState;
			m_CurrentState = new SyncStateExecuting();
			//m_Logger.addLine( std::string( getName() + " new sync state: executing" ) );

			notifyReadySet();
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void SyncStates::handleStateChangeException(Exception &e)
	{
		delete m_CurrentState;
		m_CurrentState = new SyncStateError();
		//m_Logger.addLine( std::string( getName() + " new sync state: error\n\t" + e.message() ) );
	}

	void SyncStates::resetAndEnableReadySet()
	{
		m_ReadyAccess.lock();
		for (BlockTriggerMap::iterator it = m_ReadySet.begin(); it != m_ReadySet.end(); ++it)
		{
			it->second.first->reset();
		}
		m_ReadyAccess.unlock();

		m_EnabledAccess.lock();
		m_ReadyTriggersEnabled = true;
		m_EnabledAccess.unlock();
	}

	void SyncStates::resetAndEnableFinishedSet()
	{
		m_FinishedAccess.lock();
		for (BlockTriggerMap::iterator it = m_FinishedSet.begin(); it != m_FinishedSet.end(); ++it)
		{
			it->second.first->reset();
		}
		m_FinishedAccess.unlock();

		m_EnabledAccess.lock();
		m_FinishedTriggersEnabled = true;
		m_EnabledAccess.unlock();
	}

	const bool SyncStates::areReadyTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		return m_ReadyTriggersEnabled;
	}

	const bool SyncStates::areFinishedTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		return m_FinishedTriggersEnabled;
	}

	void SyncStates::disableReadySet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_ReadyTriggersEnabled = false;
	}

	void SyncStates::disableFinishedSet()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_FinishedTriggersEnabled = false;
	}

	void SyncStates::disableAll()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_ReadyTriggersEnabled = false;
		m_FinishedTriggersEnabled = false;
	}

	void SyncStates::notifyFinishedSet()
	{
		m_FinishedAccess.lock();
		for (BlockTriggerMap::iterator it = m_FinishedSet.begin(); it != m_FinishedSet.end(); ++it)
		{
			it->second.second->tryTriggerUberBlock( *this, BLOCK_OK );
		}
		m_FinishedAccess.unlock();
	}

	void SyncStates::notifyReadySet()
	{
		m_ReadyAccess.lock();
		for (BlockTriggerMap::iterator it = m_ReadySet.begin(); it != m_ReadySet.end(); ++it)
		{
			it->second.second->tryTriggerUberBlock( *this, BLOCK_OK );
		}
		m_ReadyAccess.unlock();
	}

	void SyncStates::evaluateReadyTriggers()
	{
		bool triggersOk = true;
		for (BlockTriggerMap::iterator it = m_ReadySet.begin(); it != m_ReadySet.end(); ++it)
		{
			triggersOk &= it->second.first->ok();
		}
		if (triggersOk) disableReadySet();
		m_ReadyAccess.unlock();

		if (triggersOk)
		{
			waitForFinishedSet();
		}
	}

	void SyncStates::evaluateFinishedTriggers()
	{
		bool triggersOk = true;
		for (BlockTriggerMap::iterator it = m_FinishedSet.begin(); it != m_FinishedSet.end(); ++it)
		{
			triggersOk &= it->second.first->ok();
		}
		if (triggersOk) disableFinishedSet();
		m_FinishedAccess.unlock();

		if (triggersOk)
		{
			waitForReadySet();
		}
	}

	void SyncStates::subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
		if (desiredMsg == BLOCK_READY)
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_ReadyAccess );

			BlockTriggerMap::iterator it = m_ReadySet.find( subBlock.getName() );
			if ( it != m_ReadySet.end() )
			{
				delete it->second.first;
				m_ReadySet.erase( it );
			}

			m_ReadySet.insert( std::make_pair( subBlock.getName(), std::make_pair( &trigger, &subBlock.getStateManager() ) ) );
		}
		else if (desiredMsg == BLOCK_FINISHED)
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_FinishedAccess );

			BlockTriggerMap::iterator it = m_FinishedSet.find( subBlock.getName() );
			if ( it != m_FinishedSet.end() )
			{
				delete it->second.first;
				m_FinishedSet.erase( it );
			}

			m_FinishedSet.insert( std::make_pair( subBlock.getName(), std::make_pair( &trigger, &subBlock.getStateManager() ) ) );
		}
	}

	void SyncStates::subBlockRemoved(AbstractBlock &subBlock)
	{
		//Poco::ScopedLock< Poco::FastMutex > lock(m_TriggerAccess);
		//BlockTriggerMap::iterator it = m_SubTriggers.find(subBlock.getName());
		//if (it != m_SubTriggers.end())
		//{
		//	delete it->second.first;
		//	m_SubTriggers.erase(it);
		//}
	}

	void SyncStates::inletAdded(Inlet &slot, AbstractInletBasedTrigger &trigger)
	{
		//sync has no inlets
	}

	void SyncStates::inletRemoved(Inlet &slot)
	{
		//sync has no inlets
	}

	void SyncStates::uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
		//sync has no uber blocks
	}

	void SyncStates::uberBlockRemoved(AbstractBlock &uberBlock)
	{
		//snyc has no uber blocks
	}

	void SyncStates::tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage msg)
	{
		if ( msg == BLOCK_READY )
		{
			m_ReadyAccess.lock();
			if (!areReadyTriggersEnabled())
			{
				m_ReadyAccess.unlock();
				return;
			}

			BlockTriggerMap::iterator it = m_ReadySet.find(sub.getName());
			if (it != m_ReadySet.end() && it->second.first->tryTrigger(msg))
			{
				evaluateReadyTriggers();
			}
			else
			{
				m_ReadyAccess.unlock();
			}
		}
		else if ( msg == BLOCK_FINISHED )
		{
			m_FinishedAccess.lock();
			if (!areFinishedTriggersEnabled())
			{
				m_FinishedAccess.unlock();
				return;
			}

			BlockTriggerMap::iterator it = m_FinishedSet.find(sub.getName());
			if (it != m_FinishedSet.end() && it->second.first->tryTrigger(msg))
			{
				evaluateFinishedTriggers();
			}
			else
			{
				m_FinishedAccess.unlock();
			}
		}
	}

	void SyncStates::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
		//sync has no inlets
	}

	void SyncStates::tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg)
	{
		//sync has no uber blocks
	}

	void SyncStates::tryTriggerTime(long &time)
	{
		//sync has no time
	}

}