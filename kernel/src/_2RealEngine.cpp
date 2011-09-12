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

#include "_2RealEngine.h"
#include "_2RealEngineTypedefs.h"

#include "_2RealPluginMetadata.h"
#include "_2RealServiceMetadata.h"
#include "_2RealPluginPool.h"
#include "_2RealServiceFactory.h"
#include "_2RealEntityTable.h"
#include "_2RealProductionGraphs.h"
#include "_2RealContainer.h"
#include "_2RealEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealServiceContainer.h"
#include "_2RealServiceValue.h"
#include "_2RealServiceSlot.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	Engine *Engine::s_Instance = NULL;
	unsigned int Engine::s_iRefCount = 0;
	Poco::Mutex Engine::s_Mutex;
	
	Engine *const Engine::instance()
	{
		if (s_Mutex.tryLock(10000))
		{
			if(s_Instance == NULL)
			{
				s_Instance = new Engine();
			}
			s_iRefCount++;
			s_Mutex.unlock();
			return s_Instance;
		}

		throw Exception("internal error: could not create instance");
	}

	void Engine::retain()
	{
		if (s_Mutex.tryLock(1000))
		{
			s_iRefCount++;
			
			s_Mutex.unlock();
			return;
		}
		
		throw Exception("internal error: could not retain engine");
	}

	void Engine::release()
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
			return;
		}

		throw Exception("internal error: could not release engine");
	}

	Engine::Engine() :
		m_EntityTable(new EntityTable()),
		m_Factory(new ServiceFactory()),
		m_Plugins(new PluginPool()),
		m_Graphs(new ProductionGraphs())
	{
		m_Plugins->m_EntityTable = m_EntityTable;
		m_Plugins->m_Factory = m_Factory;
		m_Factory->m_EntityTable = m_EntityTable;
		m_Graphs->m_EntityTable = m_EntityTable;
	}

	Engine::Engine(Engine const& _src)
	{
		throw Exception("internal error: attempted to copy engine");
	}

	Engine& Engine::operator=(Engine const& _src)
	{
		throw Exception("internal error: attempted to copy engine");
	}

	Engine::~Engine()
	{
		delete m_Graphs;
		delete m_Factory;
		delete m_Plugins;
		delete m_EntityTable;
	}

	const Identifier Engine::createSystem(std::string const& _name)
	{
		try
		{
			unsigned int id = m_Graphs->createSystem(_name);
			return m_EntityTable->getIdentifier(id);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::destroySystem(Identifier const& _id)
	{
		try
		{
			m_Graphs->destroySystem(_id.id());
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier Engine::installPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifier const& _top)
	{
		try
		{
			//loads plugin & carries out init
			unsigned int id = m_Plugins->install(_name, _dir, _file, _class);
			return m_EntityTable->getIdentifier(id);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::dumpPluginInfo(Identifier const& _id, Identifier const& _top) const
	{
		try
		{
			PluginMetadata data = m_Plugins->info(_id.id());
			std::string info = data.info();
			std::cout << info << std::endl;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::dumpServiceInfo(Identifier const& _id, std::string const& _name, Identifier const& _top) const
	{
		try
		{
			ServiceMetadata data = m_Factory->info(_id.id(), _name);
			std::string info = data.info();
			std::cout << info << std::endl;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier Engine::createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifier const& _top)
	{
		try
		{
			ServiceContainer *service = m_Factory->createService(_name, _id.id(), _service);
			Container *nirvana = m_Graphs->getSystem(_top.id());
			//move into system at last index
			unsigned int index = nirvana->childCount();
			nirvana->add(service, index);
			return m_EntityTable->getIdentifier(service->id());
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers Engine::getSetupParameters(Identifier const& _id, Identifier const& _top) const
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			const AbstractContainer *container = nirvana->find(_id.id());

			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}
			else if (container->type() != Entity::SERVICE)
			{
				throw Exception("only setup params of a service can be queried");
			}

			Identifiers setup;
			IDs ids = container->setupParamIDs();
			for (IDIterator it = ids.begin(); it != ids.end(); it++)
			{
				setup.push_back(m_EntityTable->getIdentifier(*it));
			}

			return setup;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers Engine::getInputSlots(Identifier const& _id, Identifier const& _top) const
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			const AbstractContainer *container = nirvana->find(_id.id());

			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}

			Identifiers input;
			IDs ids = container->inputSlotIDs();
			for (IDIterator it = ids.begin(); it != ids.end(); it++)
			{
				input.push_back(m_EntityTable->getIdentifier(*it));
			}

			return input;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers Engine::getOutputSlots(Identifier const& _id, Identifier const& _top) const
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			const AbstractContainer *container = nirvana->find(_id.id());

			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}

			Identifiers output;
			IDs ids = container->outputSlotIDs();
			for (IDIterator it = ids.begin(); it != ids.end(); it++)
			{
				output.push_back(m_EntityTable->getIdentifier(*it));
			}

			return output;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier Engine::createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top)
	{
		try
		{
			unsigned int id = m_Graphs->createSequence(_name, _idA.id(), _idB.id(), _top.id());
			return m_EntityTable->getIdentifier(id);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier Engine::createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _top)
	{
		try
		{
			unsigned int id = m_Graphs->createSynchronization(_name, _idA.id(), _idB.id(), _top.id());
			return m_EntityTable->getIdentifier(id);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::setParameterValue(Identifier const& _id, Poco::Any _any, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			Entity *e = m_EntityTable->get(_id.id());
			ServiceValue *val = static_cast< ServiceValue * >(e);
			ServiceContainer *service = val->service();
			if (!(nirvana->find(service->id())))
			{
				throw Exception("in this system, no service has this setup param " + _id.name());
			}
			val->setValue(_any);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::link(Identifier const& _in,  Identifier const& _out, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *in = nirvana->find(_in.id());
			if (!in)
			{
				throw Exception("this system does not contain " + _in.name());
			}
			else if (in->father() != nirvana)
			{
				throw Exception(_in.name() + " is not a child of system");
			}
			AbstractContainer *out = nirvana->find(_out.id());
			if (!out)
			{
				throw Exception("this system does not contain " + _out.name());
			}
			else if (out->father() != nirvana)
			{
				throw Exception(_out.name() + " is not a child of system");
			}
			if (in == out)
			{
				throw Exception(_in.name() + " is identical to " + _out.name());
			}

			std::list< ServiceSlot * > inSlots = in->inputSlots();
			std::list< ServiceSlot * > outSlots = out->outputSlots();
			std::list< ServiceSlot * >::iterator inIt;
			std::list< ServiceSlot * >::iterator outIt;

			if (inSlots.size() != outSlots.size())
			{
				throw Exception("can not link " + _in.name() + " " + _out.name() + " , IO mismatch");
			}

			nirvana->stopChild(_in.id());
			nirvana->stopChild(_out.id());
			for (inIt = inSlots.begin(), outIt = outSlots.begin(); inIt != inSlots.end(), outIt != outSlots.end(); inIt++, outIt++)
			{
				(*outIt)->linkWith(*inIt);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::linkSlots(Identifier const& _in,  Identifier const& _out, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());

			Entity *eIn = m_EntityTable->get(_in.id());
			Entity *eOut = m_EntityTable->get(_out.id());
			ServiceSlot *in = static_cast< ServiceSlot * >(eIn);
			ServiceSlot *out = static_cast< ServiceSlot * >(eOut);
			ServiceContainer *inService = in->service();
			ServiceContainer *outService = out->service();
			
			if (!(nirvana->find(inService->id())))
			{
				throw Exception("in this system, no service has this input param " + _in.name());
			}
			else if (!(nirvana->find(outService->id())))
			{
				throw Exception("in this system, no service has this output param " + _out.name());
			}
			else if (inService == outService)
			{
				throw Exception(_in.name() + " belongs to the same service as " + _out.name());
			}

			AbstractContainer *inRoot = inService->root();
			AbstractContainer *outRoot = outService->root();
			if (inRoot == outRoot)
			{
				throw Exception(_in.name() + " belongs to the same production graph as " + _out.name());
			}

			nirvana->stopChild(inRoot->id());
			nirvana->stopChild(outRoot->id());
			out->linkWith(in);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::registerToException(Identifier const& _id, ExceptionCallback _callback, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *container = nirvana->find(_id.id());
			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}

			container->registerExceptionCallback(_callback);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::registerToNewData(Identifier const& _id, NewDataCallback _callback, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *container = nirvana->find(_id.id());
			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}

			container->registerDataCallback(_callback);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers Engine::getChildren(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			IDs ids;
			Identifiers children;
			Container *nirvana = m_Graphs->getSystem(_top.id());
			if (_id == _top)
			{
				ids = nirvana->childIDs();
			}
			else
			{
				const AbstractContainer *container = nirvana->find(_id.id());
				if (!container)
				{
					throw Exception("this system does not contain " + _id.name());
				}
				else if (container->type() == Entity::SERVICE)
				{
					throw Exception("services do not have children");
				}

				const Container *c = static_cast< const Container * >(container);
				ids = c->childIDs();
			}

			for (IDIterator it = ids.begin(); it != ids.end(); it++)
			{
				children.push_back(m_EntityTable->getIdentifier(*it));
			}
			return children;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::start(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			nirvana->startChild(_id.id());
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::startAll(Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			IDs children = nirvana->childIDs();
			for (IDIterator it = children.begin(); it != children.end(); it++)
			{
				nirvana->startChild(*it);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::stop(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			nirvana->stopChild(_id.id());
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::stopAll(Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			IDs children = nirvana->childIDs();
			for (IDIterator it = children.begin(); it != children.end(); it++)
			{
				nirvana->stopChild(*it);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *src = nirvana->get(_src.id());
			if (!src)
			{
				throw Exception("this system does not contain " + _src.name());
			}
			if (_dst.id() != _top.id())
			{
				AbstractContainer *dst = nirvana->find(_dst.id());
				if (!dst)
				{
					throw Exception("this system does not contain " + _dst.name());
				}
				Container *container = static_cast< Container * >(dst);
				container->add(src, _index);
			}
			else
			{
				nirvana->add(src, _index);
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::append(Identifier const& _dst, Identifier const& _src, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *src = nirvana->get(_src.id());
			if (!src)
			{
				throw Exception("this system does not contain " + _src.name());
			}
			if (_dst.id() != _top.id())
			{
				AbstractContainer *dst = nirvana->find(_dst.id());
				if (!dst)
				{
					throw Exception("this system does not contain " + _dst.name());
				}
				Container *container = static_cast< Container * >(dst);
				container->add(src, container->childCount());
			}
			else
			{
				nirvana->add(src, nirvana->childCount());
			}
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void Engine::destroy(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			Container *nirvana = m_Graphs->getSystem(_top.id());
			AbstractContainer *container = nirvana->get(_id.id());
			if (!container)
			{
				throw Exception("this system does not contain " + _id.name());
			}
			container->shutdown();
			m_EntityTable->destroy(container);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}