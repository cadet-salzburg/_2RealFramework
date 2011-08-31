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

#include "_2RealPluginMetadata.h"
#include "_2RealServiceMetadata.h"
#include "_2RealPluginPool.h"
#include "_2RealServiceFactory.h"
#include "_2RealEntities.h"
#include "_2RealProductionGraphs.h"
#include "_2RealIEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealServiceImpl.h"

#include <sstream>

namespace _2Real
{

	EngineImpl *EngineImpl::s_Instance = NULL;
	unsigned int EngineImpl::s_iRefCount = 0;
	Poco::Mutex EngineImpl::s_Mutex;
	
	EngineImpl *const EngineImpl::instance()
	{
		if (s_Mutex.tryLock(10000))
		{
			if(s_Instance == NULL)
			{

#ifdef _DEBUG
				std::cout << "engine (singleton): creating new instance" << std::endl;
#endif

				s_Instance = new EngineImpl();
			}

#ifdef _DEBUG
			std::cout << "engine (singleton): success" << std::endl;
#endif

			s_iRefCount++;
			s_Mutex.unlock();

#ifdef _DEBUG
			std::cout << "engine (singleton): success" << std::endl;
#endif

			return s_Instance;
		}

#ifdef _DEBUG
		std::cout << "engine: could not lock mutex for instance creation" << std::endl;
#endif

		throw Exception::failure();
	}

	void EngineImpl::retain()
	{
		if (s_Mutex.tryLock(1000))
		{
			s_iRefCount++;
			
			s_Mutex.unlock();
		}
		
		throw Exception::failure();
	}

	void EngineImpl::release()
	{
		if (s_Mutex.tryLock(1000))
		{
			s_iRefCount--;
			
			if (s_iRefCount == 0 && s_Instance != NULL)
			{
				delete s_Instance;
				s_Instance = NULL;
			}

			s_Mutex.unlock();
		}

		throw Exception::failure();
	}

	EngineImpl::EngineImpl() :
		m_Plugins(new PluginPool()),
		m_Factory(new ServiceFactory()),
		m_Entities(new Entities()),
		m_Graphs(new ProductionGraphs())
	{

#ifdef _DEBUG
		std::cout << "engine (singleton): ctor" << std::endl;
#endif

		m_Entities->m_Plugins = m_Factory->m_Plugins = m_Graphs->m_Plugins = m_Plugins;
		m_Entities->m_Factory = m_Plugins->m_Factory = m_Graphs->m_Factory = m_Factory;
		m_Entities->m_Graphs = m_Plugins->m_Graphs = m_Factory->m_Graphs = m_Graphs;
		m_Plugins->m_Entities = m_Factory->m_Entities = m_Graphs->m_Entities = m_Entities;

#ifdef _DEBUG
		std::cout << "engine (singleton): ctor success" << std::endl;
#endif

	}

	EngineImpl::EngineImpl(EngineImpl const& _src)
	{
		throw Exception::noCopy();
	}

	EngineImpl& EngineImpl::operator=(EngineImpl const& _src)
	{
		throw Exception::noCopy();
	}

	EngineImpl::~EngineImpl()
	{
		delete m_Plugins;
		delete m_Factory;
		delete m_Graphs;
		delete m_Entities;
	}

	const Identifier EngineImpl::createProductionGraph(std::string const& _name)
	{
		try
		{
			return m_Graphs->createNirvana(_name);
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::destroyProductionGraph(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			m_Graphs->destroy(_id, _top);
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::installPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifiers &_ids, Identifier const& _top)
	{

		try
		{
			return m_Plugins->install(_name, _dir, _file, _class, _ids);
		}
		catch (...)
		{

#ifdef _DEBUG
			std::cout << "engine (singleton): error on plugin loading" << std::endl;
#endif

			throw;
		}
	}

	void EngineImpl::dumpPluginInfo(Identifier const& _id) const
	{
		try
		{
			PluginMetadata data = m_Plugins->pluginInfo(_id.id());
			std::string info = data.info();
			std::cout << info << std::endl;
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::dumpServiceInfo(Identifier const& _id) const
	{
		try
		{
			ServiceMetadata data = m_Factory->serviceInfo(_id.id());
			std::string info = data.info();
			std::cout << info << std::endl;
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::dumpServiceInfo(Identifier const& _id, std::string const& _name) const
	{
		try
		{
			ServiceMetadata data = m_Factory->serviceInfo(_id.id(), _name);
			std::string info = data.info();
			std::cout << info << std::endl;
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs, Identifier const& _top)
	{
		try
		{
			return m_Factory->createService(_name, _id.id(), _setupIDs, _top);
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifiers &_setupIDs, Identifier const& _top)
	{
		try
		{
			return m_Factory->createService(_name, _id.id(), _service, _setupIDs, _top);
		}
		catch (...)
		{
			throw;
		}
	}

	Identifiers EngineImpl::getInputSlots(Identifier const& _id) const
	{
		return Identifiers();
		//try
		//{
		//	IEntity *e = m_Entities->get(_id.id());
		//	ServiceImpl *s = static_cast< ServiceImpl * >(e);
		//	return s-
		//}
		//catch (...)
		//{
		//	throw;
		//}
	}

	Identifiers EngineImpl::getOutputSlots(Identifier const& _id) const
	{
		return Identifiers();
		//try
		//{
		//	IEntity *e = m_Entities->get(_id.id());
		//	ServiceImpl *s = static_cast< ServiceImpl * >(e);
		//}
		//catch (...)
		//{
		//	throw;
		//}
	}
}