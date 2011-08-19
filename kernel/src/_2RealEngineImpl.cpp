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

#include "_2RealEngineImpl.h"

#include "_2RealPlugin.h"
#include "_2RealPluginPool.h"
#include "_2RealServiceFactory.h"
#include "_2RealEntities.h"
#include "_2RealIdentifier.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

namespace _2Real
{

	EngineImpl *EngineImpl::s_Instance = NULL;
	unsigned int EngineImpl::s_iRefCount = 0;
	Poco::Mutex EngineImpl::s_Mutex;
	
	EngineImpl *const EngineImpl::instance()
	{
		Poco::ScopedLock< Poco::Mutex > lock(s_Mutex);
#ifdef _DEBUG
		std::cout << "engine impl instance() called" << std::endl;
#endif

		if(s_Instance == NULL)
		{
			s_Instance = new EngineImpl();
		}

#ifdef _DEBUG
		std::cout << "engine impl ref count increasing" << std::endl;
#endif

		s_iRefCount++;
		return s_Instance;
	}

	const bool EngineImpl::retain()
	{
#ifdef _DEBUG
		std::cout << "engine impl ref count increasing" << std::endl;
#endif

		if (s_Mutex.tryLock(100))
		{
			s_iRefCount++;
			s_Mutex.unlock();
			return true;
		}
		return false;
	}

	const bool EngineImpl::release()
	{
#ifdef _DEBUG
		std::cout << "engine impl ref count decreasing" << std::endl;
#endif

		if (s_Mutex.tryLock(100))
		{
			s_iRefCount--;
			if (s_iRefCount == 0 && s_Instance != NULL)
			{
				delete s_Instance;
				s_Instance = NULL;
			}

			s_Mutex.unlock();
			return true;
		}

		return false;
	}

	EngineImpl::EngineImpl() : m_PluginPool(NULL), m_ServiceFactory(NULL)
	{
#ifdef _DEBUG
		std::cout << "engine impl ctor called" << std::endl;
#endif
		m_Entities = new Entities();
		m_ServiceFactory = new ServiceFactory(m_Entities);
		m_PluginPool = new PluginPool(m_ServiceFactory, m_Entities);
	}

	EngineImpl::EngineImpl(EngineImpl const& _src)
	{
		throw Exception::failure();
	}

	EngineImpl& EngineImpl::operator=(EngineImpl const& _src)
	{
		throw Exception::failure();
	}

	EngineImpl::~EngineImpl()
	{
#ifdef _DEBUG
		std::cout << "engine impl dtor called" << std::endl;
#endif

		delete m_PluginPool;
		delete m_ServiceFactory;
		delete m_Entities;
	}

	//const Identifier EngineImpl::installPlugin(std::string const& _name, std::string const& _path, std::string const& _class, std::vector< Identifier > &_serviceIDs) throw(...)
	//{
	//	try
	//	{
	//		//attempt to load & start plugin - if successfull, this causes all services to be exported by the plugin
	//		const IdentifierImpl *pluginID = m_PluginPool->install(_name, _path, _class);
	//		
	//		//services were exported, so get their factory ids
	//		std::vector< IdentifierImpl > services = m_PluginPool->get(*pluginID)->serviceIDs();
	//		_serviceIDs.clear();
	//		for (std::vector< IdentifierImpl >::iterator it = services.begin(); it != services.end(); it++)
	//		{
	//			IdentifierImpl id = *it;
	//			_serviceIDs.push_back(Identifier(&id));
	//		}

	//		//yay
	//		return Identifier(pluginID);
	//	}
	//	catch (...)
	//	{
	//		//error handling is still TODO
	//		throw;
	//	}
	//}

	//void EngineImpl::dumpPluginInfo(Identifier const& _pluginID)
	//{
	//	//metadata is TODO
	//}

	//void EngineImpl::dumpServiceInfo(Identifier const& _serviceID)
	//{
	//	//metadata is TODO
	//}

	//const Identifier EngineImpl::createProductionGraph(std::string const& _name, eContainerType const& _type) throw(...)
	//{
	//	//const IdentifierImpl *graphID = m_ServiceFactory->createProductionGraph(_name, _type);
	//	return Identifier(NULL);
	//}

	//const Identifier EngineImpl::createService(std::string const& _name, Identifier const& _id) throw(...)
	//{
	//	//const IdentifierImpl *graphID = m_ServiceFactory->createServiceContainer(_name, _id);
	//	return Identifier(NULL);
	//}

	//const Identifier EngineImpl::createMutex(std::string const& _name, Identifier const& _id) throw(...)
	//{
	//	//const IdentifierImpl *mutexID = m_ServiceFactory->get(_id)->createMutex(_name);
	//	return Identifier(NULL);
	//}

	//void EngineImpl::registerTo(Identifier const& _containerID, ExceptionCallback _callback) throw(...)
	//{
	//}

	//void EngineImpl::registerToNewDataAvailable(Identifier const& _containerID, NewDataCallback _callback) throw(...)
	//{
	//}

	//void EngineImpl::insertInto(Identifier const& _dst, Identifier const& _src) throw(...)
	//{
	//}

	//void EngineImpl::startAll() throw(...)
	//{
	//}

	//void EngineImpl::stopAll() throw(...)
	//{
	//}

}