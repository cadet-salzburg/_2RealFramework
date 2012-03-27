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

#include "_2RealSystemImpl.h"
#include "_2RealEngineImpl.h"
#include "_2RealPluginPool.h"
#include "_2RealThreadPool.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableGraph.h"
#include "_2RealExceptionHandler.h"
#include "_2RealException.h"
#include "_2RealService.h"
#include "_2RealData.h"
#include "_2RealRunnableManager.h"
#include "_2RealUpdateTriggers.h"

#include <sstream>

namespace _2Real
{

	SystemImpl::SystemImpl() :
		Graph(),
		m_PluginPool(EngineImpl::instance().getPluginPool()),
		m_ExceptionHandler(),
		m_Timestamp(),
		m_Engine(EngineImpl::instance())
	{
		m_Timestamp.update();
	}

	SystemImpl::~SystemImpl()
	{
		try
		{
			clear();
		}
		catch (TimeOutException &e)
		{
			std::cout << e.message() << std::endl;
		}
	}

	const long SystemImpl::getElapsedTime() const
	{
		return static_cast< long >(m_Timestamp.elapsed());
	}

	Poco::Timestamp const& SystemImpl::getTimestamp() const
	{
		return m_Timestamp;
	}

	void SystemImpl::clear()
	{
		RunnableList readyForDelete;
		RunnableList errorOnDelete;

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
		{
			(*it)->prepareForShutDown();
		}

		for (RunnableList::iterator it = m_Children.begin(); it != m_Children.end(); /**/)
		{
			if ((*it)->shutDown())
			{
				readyForDelete.push_back(*it);
				(*it)->debug();
			}
			else
			{
				errorOnDelete.push_back(*it);
				(*it)->debug();
			}

			it = m_Children.erase(it);
		}

		for (RunnableList::iterator it = errorOnDelete.begin(); it != errorOnDelete.end(); /**/)
		{
			(*it)->prepareForAbort();
			
			std::ostringstream msg;
			msg << (*it)->getManagedName() << " shut down failed" << std::endl;
			TimeOutException e(msg.str());
			
			(*it)->abort(e);

			it = errorOnDelete.erase(it);
		}

		for (RunnableList::iterator it = readyForDelete.begin(); it != readyForDelete.end(); /**/)
		{
			delete *it;
			it = readyForDelete.erase(it);
		}
	}

	//void SystemImpl::registerExceptionCallback(ExceptionCallback callback)
	//{
	//	m_ExceptionHandler.registerExceptionCallback(callback);
	//}

	//void SystemImpl::unregisterExceptionCallback(ExceptionCallback callback)
	//{
	//	m_ExceptionHandler.unregisterExceptionCallback(callback);
	//}

	//void SystemImpl::registerExceptionListener(IExceptionListener &listener)
	//{
	//	m_ExceptionHandler.registerExceptionListener(listener);
	//}

	//void SystemImpl::unregisterExceptionListener(IExceptionListener &listener)
	//{
	//	m_ExceptionHandler.unregisterExceptionListener(listener);
	//}

	//void SystemImpl::registerToStateChange(Identifier const& runnableId, StateChangeCallback callback)
	//{
	//	getContained(runnableId).registerToStateChange(callback);
	//}

	//void SystemImpl::unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback)
	//{
	//	getContained(runnableId).unregisterFromStateChange(callback);
	//}

	//void SystemImpl::registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	//{
	//	getContained(runnableId).registerToStateChange(listener);
	//}

	//void SystemImpl::unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	//{
	//	getContained(runnableId).unregisterFromStateChange(listener);
	//}

	void SystemImpl::registerToNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback)
	{
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.registerToNewData(outName, callback);
	}

	void SystemImpl::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, DataCallback callback)
	{
		Runnable &runnable = getContained(serviceId).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		service.unregisterFromNewData(outName, callback);
	}

	//void SystemImpl::registerToNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	//{
	//	Runnable &runnable = getContained(serviceId).getManagedRunnable();
	//	Service &service = static_cast< Service & >(runnable);
	//	service.registerToNewData(outName, listener);
	//}

	//void SystemImpl::unregisterFromNewData(Identifier const& serviceId, std::string const& outName, IOutputListener &listener)
	//{
	//	Runnable &runnable = getContained(serviceId).getManagedRunnable();
	//	Service &service = static_cast< Service & >(runnable);
	//	service.unregisterFromNewData(outName, listener);
	//}

	void SystemImpl::handleException(Runnable &runnable, Exception &exception)
	{
		//stopChild(runnable.root().identifier());
		//m_ExceptionHandler.handleException(exception, runnable.identifier());
	}

	void SystemImpl::setUp(Identifier const& id)
	{
		if (id.isService())
		{
			RunnableManager &mgr = getContained(id);
			mgr.setUp();
		}
	}

	const Identifier SystemImpl::createService(Identifier const& id, std::string const& service, UpdateTriggers const& triggers)
	{
		Runnable &runnable = m_PluginPool.createService(id, service, *this);
		RunnableManager *manager = new RunnableManager(runnable, *(triggers.m_Impl));
		m_Children.push_back(manager);
		return runnable.identifier();
	}

	const Identifier SystemImpl::createService(std::string const& name, Identifier const& id, std::string const& service, UpdateTriggers const& triggers)
	{
		Runnable &runnable = m_PluginPool.createService(name, id, service, *this);
		RunnableManager *manager = new RunnableManager(runnable, *(triggers.m_Impl));
		m_Children.push_back(manager);
		return runnable.identifier();
	}

	//const Identifier SystemImpl::createSequence(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB)
	//{
	//	const Identifier id = Entity::createIdentifier(idName, "sequence");
	//	/*MISSING*/
	//	return id;
	//}

	//const Identifier SystemImpl::createSynchronization(std::string const& idName, Identifier const& runnableA, Identifier const& runnableB)
	//{
	//	const Identifier id = Entity::createIdentifier(idName, "synchronization");
	//	/*MISSING*/
	//	return id;
	//}

	void SystemImpl::setValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		Data data(value, (long)m_Timestamp.elapsed());
		if (id.isService())
		{
			Runnable &runnable = getContained(id).getManagedRunnable();
			Service &service = static_cast< Service & >(runnable);
			service.setParameterValue(paramName, data);
		}
	}

	void SystemImpl::sendValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		Data data(value, (long)m_Timestamp.elapsed());
		if (id.isService())
		{
			Runnable &runnable = getContained(id).getManagedRunnable();
			Service &service = static_cast< Service & >(runnable);
			service.insertParameterValue(paramName, data);
		}
	}

	const EngineData SystemImpl::getValue(Identifier const& id, std::string const& paramName) const
	{
		Runnable &runnable = getContained(id).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		return service.getParameterValue(paramName);
	}

	std::string const& SystemImpl::getParameterKey(Identifier const& id, std::string const& paramName) const
	{
		Runnable &runnable = getContained(id).getManagedRunnable();
		Service &service = static_cast< Service & >(runnable);
		return service.getParameterKey(paramName);
	}

	void SystemImpl::linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut)
	{
		Runnable &runnableIn = getContained(serviceIn).getManagedRunnable();
		Service &in = static_cast< Service & >(runnableIn);

		Runnable &runnableOut = getContained(serviceOut).getManagedRunnable();
		Service &out = static_cast< Service & >(runnableOut);

		in.linkWith(nameIn, out, nameOut);
	}

}