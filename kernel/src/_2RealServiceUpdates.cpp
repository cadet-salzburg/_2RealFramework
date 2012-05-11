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

#include "_2RealServiceUpdates.h"
#include "_2RealInputSlot.h"
#include "_2RealServiceStates.h"
#include "_2RealServiceIO.h"
#include "_2RealUpdatePolicyImpl.h"
#include "_2RealEngineImpl.h"
#include "_2RealTimer.h"
#include "_2RealAbstractBlock.h"

namespace _2Real
{

	//ServiceUpdates::ServiceUpdates(AbstractBlock &owner) :
	//	AbstractUpdateManager(owner),
	//	m_States(NULL),
	//	m_Time(NULL),
	//	m_UberTriggersEnabled(false),
	//	m_TriggersEnabled(false)
	//{
	//}

	//ServiceUpdates::~ServiceUpdates()
	//{
	//	std::cout << getName() << " service updates: dtor" << std::endl;
	//	clear();
	//	std::cout << getName() << " service updates: cleared" << std::endl;
	//}

	//void ServiceUpdates::initFrom(UpdatePolicyImpl const& triggers)
	//{
	//	AbstractTimeBasedTrigger *t = triggers.getTimeBasedTrigger();
	//	if (t != NULL)
	//	{
	//		EngineImpl::instance().getTimer().registerToTimerSignal(*this);
	//		m_Time = t;
	//	}
	//	else
	//	{
	//		//i guess this should always be fulfilled
	//		EngineImpl::instance().getTimer().registerToTimerSignal(*this);
	//		m_Time = new TimeBasedTrigger< std::less_equal< long > >(LONG_MAX);
	//	}

	//	InletMap inlets = m_IO->getInlets();
	//	for (InletMap::const_iterator it = inlets.begin(); it != inlets.end(); ++it)
	//	{
	//		it->second->registerToDataReceived(*this);
	//		AbstractInletBasedTrigger *t = triggers.getInletBasedTrigger(it->second->getName());
	//		m_Inlets.insert(std::make_pair(t->getInletName(), t));
	//	}
	//}

	//void ServiceUpdates::clear()
	//{
	//	std::cout << getName() << " service updates: clearing" << std::endl;

	//	disableAll();

	//	m_TriggerAccess.lock();

	//	if (m_Time != NULL)
	//	{
	//		EngineImpl::instance().getTimer().unregisterFromTimerSignal(*this);
	//		delete m_Time;
	//		m_Time = NULL;
	//	}

	//	InletMap inlets = m_IO->getInlets();

	//	for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); /**/)
	//	{
	//		InletMap::iterator ioIt = inlets.find(it->first);
	//		if (ioIt != inlets.end())
	//		{
	//			ioIt->second->unregisterFromDataReceived(*this);
	//		}
	//		
	//		delete it->second;
	//		it = m_Inlets.erase(it);
	//	}

	//	for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); /**/)
	//	{
	//		std::cout << getName() << " service updates: deleting trigger " << it->second.first->getBlockName() << std::endl;
	//		delete it->second.first;
	//		it = m_UberTriggers.erase(it);
	//	}

	//	m_TriggerAccess.unlock();
	//}

	//void ServiceUpdates::uberBlockAdded(AbstractBlock &uberBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_UberTriggerAccess);

	//	TriggerMap::iterator it = m_UberTriggers.find(uberBlock.getName());
	//	if (it != m_UberTriggers.end())
	//	{
	//		delete it->second.first;
	//		m_UberTriggers.erase(it);
	//	}

	//	m_UberTriggers.insert(std::make_pair(uberBlock.getName(), std::make_pair(&trigger, &uberBlock.getStateManager())));
	//}

	//void ServiceUpdates::uberBlockRemoved(AbstractBlock &uberBlock)
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_UberTriggerAccess);

	//	TriggerMap::iterator it = m_UberTriggers.find(uberBlock.getName());
	//	if (it != m_UberTriggers.end())
	//	{
	//		delete it->second.first;
	//		m_UberTriggers.erase(it);
	//	}
	//}

	//void ServiceUpdates::subBlockAdded(AbstractBlock &subBlock, AbstractBlockBasedTrigger &trigger, const BlockMessage desiredMsg)
	//{
	//	//service has no sub blocks
	//}

	//void ServiceUpdates::subBlockRemoved(AbstractBlock &subBlock)
	//{
	//	//service has no sub blocks
	//}

	//void ServiceUpdates::inletAdded(InputSlot &slot, AbstractInletBasedTrigger &trigger)
	//{
	//	//service cannot gain or loose inlets
	//}

	//void ServiceUpdates::inletRemoved(InputSlot &slot)
	//{
	//	//service cannot gain or loose inlets
	//}

	//void ServiceUpdates::tryTriggerInlet(const void *inlet, std::pair< long, long > &times)
	//{
	//	if (areTriggersEnabled())
	//	{
	//		InputSlot const* in = static_cast< InputSlot const* >(inlet);
	//		m_TriggerAccess.lock();
	//		std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.find(in->getName());
	//		if (it != m_Inlets.end() && it->second->tryTrigger(times.first, times.second))
	//		{
	//			//m_TriggerAccess.unlock();
	//			evaluateTriggers();
	//		}
	//		else
	//		{
	//			m_TriggerAccess.unlock();
	//		}
	//	}
	//}

	//void ServiceUpdates::tryTriggerSubBlock(AbstractUpdateManager &sub, const BlockMessage msg)
	//{
	//	//service has no sub blocks
	//}

	//void ServiceUpdates::tryTriggerTime(long &time)
	//{
	//	if (areTriggersEnabled())
	//	{
	//		m_TriggerAccess.lock();
	//		if (m_Time != NULL && m_Time->tryTrigger(time))
	//		{
	//			//m_TriggerAccess.unlock();
	//			evaluateTriggers();
	//		}
	//		else
	//		{
	//			m_TriggerAccess.unlock();
	//		}
	//	}
	//}

	//void ServiceUpdates::tryTriggerUberBlock(AbstractUpdateManager &uber, const BlockMessage msg)
	//{
	//	if (areUberTriggersEnabled())
	//	{
	//		m_UberTriggerAccess.lock();
	//		TriggerMap::iterator it = m_UberTriggers.find(uber.getName());
	//		if (it != m_UberTriggers.end() && it->second.first->tryTrigger(msg))
	//		{
	//			//m_UberTriggerAccess.unlock();
	//			evaluateUberTriggers();
	//		}
	//		else
	//		{
	//			m_UberTriggerAccess.unlock();
	//		}
	//	}
	//}

	//void ServiceUpdates::notifyUberTriggers( const BlockMessage msg )
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock( m_UberTriggerAccess );
	//	for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
	//	{
	//		std::cout << getName() << " trying to trigger " << it->first << std::endl;
	//		it->second.second->tryTriggerSubBlock( *this, msg );
	//	}
	//}

	//void ServiceUpdates::evaluateTriggers()
	//{
	//	//m_TriggerAccess.lock();
	//	bool triggersOk = m_Time->ok();
	//	for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
	//	{
	//		triggersOk &= it->second->ok();
	//	}
	//	if ( triggersOk ) disableTriggers();
	//	m_TriggerAccess.unlock();

	//	if (triggersOk)
	//	{
	//		m_States->triggersReady();
	//	}
	//}

	//void ServiceUpdates::evaluateUberTriggers()
	//{
	//	//m_UberTriggerAccess.lock();
	//	bool triggersOk = true;
	//	for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
	//	{
	//		triggersOk &= it->second.first->ok();
	//	}
	//	if ( triggersOk ) disableUberTriggers();
	//	m_UberTriggerAccess.unlock();

	//	if (triggersOk)
	//	{
	//		m_States->beginUpdate();
	//	}
	//}

	//void ServiceUpdates::resetAndEnableTriggers()
	//{
	//	m_TriggerAccess.lock();
	//	m_Time->reset();
	//	for (std::map< std::string, AbstractInletBasedTrigger * >::iterator it = m_Inlets.begin(); it != m_Inlets.end(); ++it)
	//	{
	//		it->second->reset();
	//	}
	//	m_TriggerAccess.unlock();

	//	m_EnabledAccess.lock();
	//	m_TriggersEnabled = true;
	//	m_EnabledAccess.unlock();
	//}

	//void ServiceUpdates::resetAndEnableUberTriggers()
	//{
	//	m_UberTriggerAccess.lock();
	//	for (TriggerMap::iterator it = m_UberTriggers.begin(); it != m_UberTriggers.end(); ++it)
	//	{
	//		it->second.first->reset();
	//	}
	//	m_UberTriggerAccess.unlock();

	//	
	//}

	//const bool ServiceUpdates::areTriggersEnabled() const
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
	//	return m_TriggersEnabled;
	//}

	//const bool ServiceUpdates::areUberTriggersEnabled() const
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
	//	return m_UberTriggersEnabled;
	//}

	//void ServiceUpdates::disableAll()
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
	//	m_TriggersEnabled = false;
	//	m_UberTriggersEnabled = false;
	//}

	//void ServiceUpdates::disableTriggers()
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
	//	m_TriggersEnabled = false;
	//}

	//void ServiceUpdates::disableUberTriggers()
	//{
	//	Poco::ScopedLock< Poco::FastMutex > lock(m_EnabledAccess);
	//	m_UberTriggersEnabled = false;
	//}

}