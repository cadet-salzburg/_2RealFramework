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
#include "_2RealContainer.h"
#include "_2RealIEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealServiceImpl.h"
#include "_2RealServiceValue.h"
#include "_2RealServiceSlot.h"

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
			unsigned int id = m_Graphs->createNirvana(_name);
			return m_Entities->getIdentifier(id);
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
			m_Graphs->destroy(_id.id(), _top.id());
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
			std::list< unsigned int > ids;
			unsigned int id = m_Plugins->install(_name, _dir, _file, _class, ids);
			for (std::list< unsigned int >::iterator it = ids.begin(); it != ids.end(); it++)
			{
				_ids.push_back(m_Entities->getIdentifier(*it));
			}
			return m_Entities->getIdentifier(id);
		}
		catch (...)
		{
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

	const Identifier EngineImpl::createService(std::string const& _name, Identifier const& _id, Identifiers &_ids, Identifier const& _top)
	{
		try
		{
			std::list< unsigned int > ids;
			ids.clear();

			unsigned int id = m_Factory->createService(_name, _id.id(), ids, _top.id());
			std::cout << ids.size() << std::endl;

			for (std::list< unsigned int >::iterator it = ids.begin(); it != ids.end(); it++)
			{
				std::cout << "TRANSFORMING setup value" << std::endl;
				_ids.push_back(m_Entities->getIdentifier(*it));
			}
			
			return m_Entities->getIdentifier(id);
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifiers &_ids, Identifier const& _top)
	{
		try
		{
			std::list< unsigned int > ids;
			ids.clear();

			unsigned int id = m_Factory->createService(_name, _id.id(), _service, ids, _top.id());
			std::cout << ids.size() << std::endl;

			for (std::list< unsigned int >::iterator it = ids.begin(); it != ids.end(); it++)
			{
				std::cout << "TRANSFORMING setup value" << std::endl;
				_ids.push_back(m_Entities->getIdentifier(*it));
			}

			return m_Entities->getIdentifier(id);
		}
		catch (...)
		{
			throw;
		}
	}

	Identifiers EngineImpl::getInputSlots(Identifier const& _id) const
	{
		try
		{
			std::list< unsigned int > slots;
			std::list< Identifier > ids;
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *c = static_cast< ServiceImpl * >(e);
			slots = c->inputParams();
			for (std::list< unsigned int >::iterator it = slots.begin(); it != slots.end(); it++)
			{
				ids.push_back(m_Entities->getIdentifier(*it));
			}
			return ids;
		}
		catch (...)
		{
			throw;
		}
	}

	Identifiers EngineImpl::getOutputSlots(Identifier const& _id) const
	{
		try
		{
			std::list< unsigned int > slots;
			std::list< Identifier > ids;
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *c = static_cast< ServiceImpl * >(e);
			slots = c->outputParams();
			for (std::list< unsigned int >::iterator it = slots.begin(); it != slots.end(); it++)
			{
				ids.push_back(m_Entities->getIdentifier(*it));
			}
			return ids;
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::createSequenceContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top)
	{
		try
		{
			unsigned int id = m_Graphs->createSequence(_name, _idA.id(), _idB.id(), _top.id());
			return m_Entities->getIdentifier(id);
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier EngineImpl::createSynchronizationContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top)
	{
		try
		{
			unsigned int id = m_Graphs->createSynchronization(_name, _idA.id(), _idB.id(), _top.id());
			return m_Entities->getIdentifier(id);
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::setParameterValue(Identifier const& _id, Poco::Any _any)
	{
		try
		{
			IEntity *e = m_Entities->get(_id.id());
			ServiceValue *val = static_cast< ServiceValue * >(e);
			val->setValue(_any);
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::link(Identifier const& _in,  Identifier const& _out)
	{
		try
		{
			IEntity *in = m_Entities->get(_in.id());
			IEntity *out = m_Entities->get(_out.id());
			ServiceSlot *inSlot = static_cast< ServiceSlot * >(in);
			ServiceSlot *outSlot = static_cast< ServiceSlot * >(out);
			ServiceImpl *inService = inSlot->service();
			ServiceImpl *outService = outSlot->service();

			//TODO: some checks
			//like if there is a cycle

			//first, find out if in was already linked
			//& reset if it was
			if (inSlot->isLinked())
			{
				ServiceSlot *senderSlot = inSlot->linked();
				inSlot->reset();
				senderSlot->reset();
				ServiceImpl *senderService = senderSlot->service();
				senderService->removeListener(inService);
				inService->stopListeningTo(senderService);
				//sender service now sends its data nowhere
			}

			//second, find out if out was already linked
			//& reset if it was
			if (outSlot->isLinked())
			{
				ServiceSlot *listenerSlot = outSlot->linked();
				outSlot->reset();
				listenerSlot->reset();
				ServiceImpl *listenerService = listenerSlot->service();
				outService->removeListener(listenerService);
				listenerService->stopListeningTo(outService);
				//listener service now does not receive anything anymore
			}

			//connect in & out
			inSlot->linkWith(outSlot);
			outSlot->linkWith(inSlot);
			outService->addListener(inService);
			inService->listenTo(outService);
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::registerToException(Identifier const& _id, ExceptionCallback _callback)
	{
		try
		{
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *container = static_cast< AbstractContainer * >(e);
			AbstractContainer *father = container->father();
			if (m_Graphs->isNirvana(father->id()))
			{
				container->registerExceptionCallback(_callback);
			}
			else
			{
				throw Exception::failure();
			}
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::registerToNewData(Identifier const& _id, NewDataCallback _callback)
	{
		try
		{
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *container = static_cast< AbstractContainer * >(e);
			AbstractContainer *father = container->father();
			if (m_Graphs->isNirvana(father->id()))
			{
				container->registerDataCallback(_callback);
			}
			else
			{
				throw Exception::failure();
			}
		}
		catch (...)
		{
			throw;
		}
	}

	Identifiers EngineImpl::getChildren(Identifier const& _id)
	{
		try
		{
			std::list< unsigned int > children;
			std::list< Identifier > ids;
			children.clear();
			ids.clear();

			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *container = static_cast< AbstractContainer * >(e);
			Container *father = static_cast< Container * >(container);
			children = father->children();
			for (std::list< unsigned int >::iterator it = children.begin(); it != children.end(); it++)
			{
				ids.push_back(m_Entities->getIdentifier(*it));
			}
			return ids;
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::start(Identifier const& _id)
	{
		try
		{
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *container = static_cast< AbstractContainer * >(e);
			Container *father = static_cast< Container * >(container->father());
			if (m_Graphs->isNirvana(father->id()))
			{
				//this is nirvana. will create thread & start child
				father->startChild(_id.id());
			}
			else
			{
				throw Exception::failure();
			}
		}
		catch (...)
		{
			throw;
		}
	}

	void EngineImpl::stop(Identifier const& _id)
	{
		try
		{
			IEntity *e = m_Entities->get(_id.id());
			AbstractContainer *container = static_cast< AbstractContainer * >(e);
			Container *father = static_cast< Container * >(container->father());
			if (m_Graphs->isNirvana(father->id()))
			{
				//this is nirvana. will create thread & start child
				father->stopChild(_id.id());
			}
			else
			{
				throw Exception::failure();
			}
		}
		catch (...)
		{
			throw;
		}
	}
}