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

#include "_2RealException.h"
#include "_2RealServiceBlock.h"
#include "_2RealAbstractServiceState.h"
#include "_2RealServiceStateCreated.h"
#include "_2RealServiceStateSetUp.h"
#include "_2RealServiceStateUpdating.h"
#include "_2RealServiceStateExecuting.h"
#include "_2RealServiceStatePreShutDown.h"
#include "_2RealServiceStateShutDown.h"
#include "_2RealServiceStateError.h"
#include "_2RealAbstractIOManager.h"
#include "_2RealEngineImpl.h"
#include "_2RealThreadPool.h"
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealTimer.h"
#include "_2RealInputSlot.h"
#include "_2RealLogger.h"

namespace _2Real
{

	ServiceStates::ServiceStates(AbstractBlock &owner) :
		AbstractStateManager(owner),
		m_CurrentState(new ServiceStateCreated()),
		m_FlaggedForSetUp(false),
		m_FlaggedForShutDown(false),
		m_FlaggedForAbort(false),
		m_StopEvent(false),
		m_TriggersEnabled(false),
		m_UberTriggersEnabled(false),
		m_Time(NULL),
		m_IO(NULL),
		m_Threads(EngineImpl::instance().getThreadPool()),
		m_Logger(EngineImpl::instance().getLogger())
	{
	}

	ServiceStates::~ServiceStates()
	{
		clear();
	}

	void ServiceStates::clear()
	{
		disableAllTriggers();

		if (m_CurrentState != NULL)
		{
			delete m_CurrentState;
			m_CurrentState = NULL;
		}

		if (m_Time != NULL)
		{
			EngineImpl::instance().getTimer().unregisterFromTimerSignal(*this);
			delete m_Time;
			m_Time = NULL;
		}

		InletMap inlets = m_IO->getInlets();

		for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
		{
			InletMap::iterator ioIt = inlets.find(it->first);
			if (ioIt != inlets.end())
			{
				ioIt->second->unregisterFromDataReceived(*this);
			}
			
			delete it->second;
			it = m_Inlets.erase(it);
		}

		for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); /**/)
		{
			delete it->second.first;
			it = m_UberTriggers.erase(it);
		}
	}

	void ServiceStates::flagForSetUp()
	{
		m_FlaggedForSetUp = true;
	}

	void ServiceStates::flagForShutDown()
	{
		m_FlaggedForShutDown = true;
	}

	void ServiceStates::flagStopped()
	{
		m_StopEvent.set();
	}

	const bool ServiceStates::conditionalSetUp()
	{
		if (m_FlaggedForShutDown)
		{
			m_StopEvent.set();
			return false;
		}

		if (m_FlaggedForSetUp)
		{
			setUpService();
		}

		resetAndEnableTriggers();
		m_IO->updateInlets();
		return true;
	}

	void ServiceStates::executeService()
	{
		m_IO->syncInlets();

		dynamic_cast< ServiceBlock & >(m_Owner).executeService();

		m_IO->updateOutlets();
	}

	void ServiceStates::setUpService()
	{
		dynamic_cast< ServiceBlock & >(m_Owner).setUpService();
	}

	void ServiceStates::shutDownService()
	{
		dynamic_cast< ServiceBlock & >(m_Owner).shutDownService();
	}

	void ServiceStates::setUp()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			if (m_CurrentState->setUp(*this))
			{
				// only if true is returned, the state actually changes
				// otherwise, the service is currently in an execution cycle -> set up will be carried out later
				delete m_CurrentState;
				m_CurrentState = new ServiceStateSetUp();
				//m_Logger.addLine( std::string( getName() + " new service state: set up (user called)" ) );

				//also, in newly setup state, triggers are needed!
				resetAndEnableTriggers();
			}
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void ServiceStates::triggersReady()
	{
		try
		{
			m_StateAccess.lock();
			m_CurrentState->triggersReady(*this);
			delete m_CurrentState;
			m_CurrentState = new ServiceStateWaiting();
			//m_Logger.addLine( std::string( getName() + " new service state: waiting" ) );
			m_StateAccess.unlock();

			if ( hasUberTriggers() )
			{
				resetAndEnableUberTriggers();
				notifyUberTriggers( BLOCK_READY );
			}
			else
			{
				beginUpdate();
			}
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void ServiceStates::beginUpdate()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->beginUpdate(*this);

			delete m_CurrentState;
			m_CurrentState = new ServiceStateUpdating();
			//m_Logger.addLine( std::string( getName() + " new service state: updating" ) );
			m_Threads.scheduleService(*this);
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void ServiceStates::beginExecution()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			m_CurrentState->beginExecution(*this);
			delete m_CurrentState;
			m_CurrentState = new ServiceStateExecuting();
			//m_Logger.addLine( std::string( getName() + " new service state: executing" ) );
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void ServiceStates::finishExecution()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			if (m_CurrentState->finishExecution(*this))
			{
				//if true is returned: change into 'set up' state
				delete m_CurrentState;
				m_CurrentState = new ServiceStateSetUp();
				//m_Logger.addLine( std::string( getName() + " new service state: setup (finished exec)" ) );

				if ( hasUberTriggers() )
				{
					//this signals to the uber trigger, that it should change its state
					notifyUberTriggers( BLOCK_FINISHED );
				}
			}
			else
			{
				//if true is returned: change into 'pre shut down' state
				delete m_CurrentState;
				m_CurrentState = new ServiceStatePreShutDown();
				//m_Logger.addLine( std::string( getName() + " new service state: prepared to shut down" ) );
			}
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	void ServiceStates::prepareForShutDown()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_StateAccess);
		try
		{
			if (m_CurrentState->prepareForShutDown(*this))
			{
				//only if true is returned, the state actually changes
				delete m_CurrentState;
				m_CurrentState = new ServiceStatePreShutDown();
				//m_Logger.addLine( std::string( getName() + " new service state: prepared to shut down" ) );
			}
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
		}
	}

	const bool ServiceStates::shutDown()
	{
		try
		{
			if (m_StopEvent.tryWait(1000))
			{
				m_StateAccess.lock();
				m_CurrentState->shutDown(*this);
				delete m_CurrentState;
				m_CurrentState = new ServiceStateShutDown();
				//m_Logger.addLine( std::string( getName() + " new service state: shut down" ) );
				m_StateAccess.unlock();

				return true;
			}
			else
			{
				m_StateAccess.lock();
				delete m_CurrentState;
				m_CurrentState = new ServiceStateError();
				//m_Logger.addLine( std::string( getName() + " new service state: error (aborted)" ) );
				m_StateAccess.unlock();

				m_Threads.abortService(*this);
				return false;
			}
		}
		catch (Exception &e)
		{
			handleStateChangeException(e);
			m_StateAccess.unlock();
			return false;
		}
	}

	void ServiceStates::handleStateChangeException(Exception &e)
	{
		std::cout << getName() << " ERROR " << e.message() << std::endl;
		disableAllTriggers();	//is this a good idea? -> i probably would hav to tell ubers..
		delete m_CurrentState;
		m_CurrentState = new ServiceStateError();
		m_Logger.addLine( std::string( getName() + " new service state: error\n\t" + e.message() ) );
	}

	void ServiceStates::initFrom(UpdatePolicyImpl const& triggers)
	{
		AbstractTimeBasedTrigger *t = triggers.getTimeBasedTrigger();
		if (t != NULL)
		{
			EngineImpl::instance().getTimer().registerToTimerSignal(*this);
			m_Time = t;
		}
		else
		{
			//i guess this should always be fulfilled
			EngineImpl::instance().getTimer().registerToTimerSignal(*this);
			m_Time = new TimeBasedTrigger< std::less_equal< long > >(LONG_MAX);
		}

		InletMap inlets = m_IO->getInlets();
		for (InletMap::const_iterator it = inlets.begin(); it != inlets.end(); ++it)
		{
			it->second->registerToDataReceived(*this);
			AbstractInletBasedTrigger *t = triggers.getInletBasedTrigger(it->second->getName());
			m_Inlets.insert(std::make_pair(t->getInletName(), t));
		}
	}

	void ServiceStates::uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_UberTriggerAccess);

		TriggerMap::iterator it = m_UberTriggers.find(uberBlock.getName());
		if (it != m_UberTriggers.end())
		{
			delete it->second.first;
			m_UberTriggers.erase(it);
		}

		m_UberTriggers.insert(std::make_pair(uberBlock.getName(), std::make_pair(&trigger, &uberBlock.getStateManager())));
	}

	void ServiceStates::uberBlockRemoved(AbstractBlock &uberBlock)
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_UberTriggerAccess);

		TriggerMap::iterator it = m_UberTriggers.find(uberBlock.getName());
		if (it != m_UberTriggers.end())
		{
			delete it->second.first;
			m_UberTriggers.erase(it);
		}
	}

	void ServiceStates::subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	{
		//service has no sub blocks
	}

	void ServiceStates::subBlockRemoved(AbstractBlock &subBlock)
	{
		//service has no sub blocks
	}

	//void ServiceStates::inletAdded(InputSlot &slot, AbstractInletBasedTrigger &trigger)
	//{
	//	//service cannot gain or loose inlets
	//}

	//void ServiceStates::inletRemoved(InputSlot &slot)
	//{
	//	//service cannot gain or loose inlets
	//}

	void ServiceStates::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	{
		if (areTriggersEnabled())
		{
			InputSlot const* in = static_cast< InputSlot const* >(inlet);
			m_TriggerAccess.lock();
			std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.find(in->getName());
			if (it != m_Inlets.end() && it->second->tryTrigger(times.first, times.second))
			{
				//m_TriggerAccess.unlock();
				evaluateTriggers();
			}
			else
			{
				m_TriggerAccess.unlock();
			}
		}
	}

	void ServiceStates::tryTriggerSubBlock(AbstractStateManager &sub, const BlockMessage msg)
	{
		//service has no sub blocks
	}

	void ServiceStates::tryTriggerTime(long &time)
	{
		if (areTriggersEnabled())
		{
			m_TriggerAccess.lock();
			if (m_Time != NULL && m_Time->tryTrigger(time))
			{
				//m_TriggerAccess.unlock();
				evaluateTriggers();
			}
			else
			{
				m_TriggerAccess.unlock();
			}
		}
	}

	void ServiceStates::tryTriggerUberBlock(AbstractStateManager &uber, const BlockMessage msg)
	{
		if (areUberTriggersEnabled())
		{
			m_UberTriggerAccess.lock();
			TriggerMap::iterator it = m_UberTriggers.find(uber.getName());
			if (it != m_UberTriggers.end() && it->second.first->tryTrigger(msg))
			{
				//m_UberTriggerAccess.unlock();
				evaluateUberTriggers();
			}
			else
			{
				m_UberTriggerAccess.unlock();
			}
		}
	}

	void ServiceStates::notifyUberTriggers( const BlockMessage msg )
	{
		Poco::ScopedLock< Poco::FastMutex > lock( m_UberTriggerAccess );
		for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
		{
			it->second.second->tryTriggerSubBlock( *this, msg );
		}
	}

	void ServiceStates::evaluateTriggers()
	{
		//m_TriggerAccess.lock();
		bool triggersOk = m_Time->ok();
		for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
		{
			triggersOk &= it->second->ok();
		}
		if ( triggersOk ) disableTriggers();
		m_TriggerAccess.unlock();

		if (triggersOk)
		{
			triggersReady();
		}
	}

	void ServiceStates::evaluateUberTriggers()
	{
		//m_UberTriggerAccess.lock();
		bool triggersOk = true;
		for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
		{
			triggersOk &= it->second.first->ok();
		}
		if ( triggersOk ) disableUberTriggers();
		m_UberTriggerAccess.unlock();

		if (triggersOk)
		{
			beginUpdate();
		}
	}

	void ServiceStates::resetAndEnableTriggers()
	{
		m_TriggerAccess.lock();
		m_Time->reset();
		for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
		{
			it->second->reset();
		}
		m_TriggerAccess.unlock();

		m_EnabledAccess.lock();
		m_TriggersEnabled = true;
		m_EnabledAccess.unlock();
	}

	void ServiceStates::resetAndEnableUberTriggers()
	{
		m_UberTriggerAccess.lock();
		for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
		{
			it->second.first->reset();
		}
		m_UberTriggerAccess.unlock();

		m_EnabledAccess.lock();
		m_UberTriggersEnabled = true;
		m_EnabledAccess.unlock();
	}

	const bool ServiceStates::areTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		return m_TriggersEnabled;
	}

	const bool ServiceStates::areUberTriggersEnabled() const
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		return m_UberTriggersEnabled;
	}

	void ServiceStates::disableAllTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_TriggersEnabled = false;
		m_UberTriggersEnabled = false;
	}

	void ServiceStates::disableTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_TriggersEnabled = false;
	}

	void ServiceStates::disableUberTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
		m_UberTriggersEnabled = false;
	}

	bool ServiceStates::hasUberTriggers()
	{
		Poco::ScopedLock< Poco::FastMutex > lock(m_UberTriggerAccess);
		return !m_UberTriggers.empty();
	}

}