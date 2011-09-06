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
#include "_2RealEngineImpl.h"
#include "_2RealException.h"
#include "_2RealIdentifier.h"

#include <iostream>

namespace _2Real
{

	Engine::Engine(std::string const& _name) : m_Impl(NULL), m_ID(NULL)
	{
		try
		{
			m_Impl = EngineImpl::instance();
			m_ID = new Identifier(m_Impl->createProductionGraph(_name));
		}
		catch (...)
		{

			if (m_Impl)
			{
				m_Impl->release();
				m_Impl = NULL;
			}

			m_ID = NULL;

			throw;
		}
	}

	Engine::Engine(Engine const& _src) : m_Impl(NULL), m_ID(NULL)
	{
		m_Impl = EngineImpl::instance();
		
		if (_src.m_ID)
		{
			m_ID = new Identifier(*_src.m_ID);
		}
	}
	
	Engine& Engine::operator=(Engine const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		if (!m_Impl)
		{
			m_Impl = EngineImpl::instance();
		}

		if (m_ID)
		{
			//this will delete everything inside of the graph
			m_Impl->destroyProductionGraph(*m_ID, *m_ID);
		}

		if (_src.m_ID)
		{
			m_ID = new Identifier(*_src.m_ID);
		}

		return *this;
	}

	Engine::~Engine()
	{
		m_Impl->destroyProductionGraph(*m_ID, *m_ID);
		delete m_ID;
		m_Impl->release();
	}

	const Identifier Engine::loadPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifiers &_serviceIDs)
	{
		try
		{
			return m_Impl->installPlugin(_name, _dir, _file, _class, _serviceIDs, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on loadPlugin()" << std::endl;
			throw;
		}
	}

	void Engine::dumpPluginInfo(Identifier const& pluginID)
	{
		try
		{
			m_Impl->dumpPluginInfo(pluginID);
		}
		catch (...)
		{
			std::cout << "engine: error on dumpPluginInfo()" << std::endl;
			throw;
		}
	}

	void Engine::dumpServiceInfo(Identifier const& serviceID)
	{
		try
		{
			m_Impl->dumpServiceInfo(serviceID);
		}
		catch (...)
		{
			std::cout << "engine: error on dumpServiceInfo()" << std::endl;
			throw;
		}
	}

	void Engine::dumpServiceInfo(Identifier const& pluginID, std::string const& name)
	{
		try
		{
			m_Impl->dumpServiceInfo(pluginID, name);
		}
		catch (...)
		{
			std::cout << "engine: error on dumpServiceInfo()" << std::endl;
			throw;
		}
	}

	const Identifier Engine::createService(std::string const& _name, Identifier const& _id, Identifiers &_setupIDs)
	{
		try
		{
			return m_Impl->createService(_name, _id, _setupIDs, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on createService()" << std::endl;
			throw;
		}
	}

	const Identifier Engine::createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifiers &_setupIDs)
	{
		try
		{
			return m_Impl->createService(_name, _id, _service, _setupIDs, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on createService()" << std::endl;
			throw;
		}
	}

	Identifiers Engine::getInputSlots(Identifier const& _id)
	{
		try
		{
			return m_Impl->getInputSlots(_id);
		}
		catch (...)
		{
			std::cout << "engine: error on getInputSlots()" << std::endl;
			throw;
		}
	}

	Identifiers Engine::getOutputSlots(Identifier const& _id)
	{
		try
		{
			return m_Impl->getOutputSlots(_id);
		}
		catch (...)
		{
			std::cout << "engine: error on getOutputSlots()" << std::endl;
			throw;
		}
	}

	const Identifier Engine::createSequenceContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	{
		try
		{
			return m_Impl->createSequenceContainer(_name, _idA, _idB, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on createSequenceContainer()" << std::endl;
			throw;
		}
	}

	const Identifier Engine::createSynchronizationContainer(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	{
		try
		{
			return m_Impl->createSynchronizationContainer(_name, _idA, _idB, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on createSynchronizationContainer()" << std::endl;
			throw;
		}
	}

	void Engine::setParameterValue(Identifier const& _id, Poco::Any _any)
	{
		try
		{
			m_Impl->setParameterValue(_id, _any);
		}
		catch (...)
		{
			std::cout << "engine: error on setParameterValue()" << std::endl;
			throw;
		}
	}

	void Engine::link(Identifier const& _in, Identifier const& _out)
	{
		try
		{
			m_Impl->link(_in, _out);
		}
		catch (...)
		{
			std::cout << "engine: error on link()" << std::endl;
			throw;
		}
	}

	void Engine::registerToException(Identifier const& _id, ExceptionCallback _callback)
	{
		try
		{
			//m_Impl->registerToException(_id, _callback);
		}
		catch (...)
		{
			std::cout << "engine: error on registerToException()" << std::endl;
			throw;
		}
	}

	void Engine::registerToNewData(Identifier const& _id, NewDataCallback _callback)
	{
		try
		{
			//m_Impl->registerToNewData(_id, _callback);
		}
		catch (...)
		{
			std::cout << "engine: error on registerToNewData()" << std::endl;
			throw;
		}
	}

	Identifiers Engine::getChildren(Identifier const& _id)
	{
		try
		{
			return m_Impl->getChildren(_id);
		}
		catch (...)
		{
			std::cout << "engine: error on getChildren()" << std::endl;
			throw;
		}
	}

	void Engine::start(Identifier const& _id)
	{
		try
		{
			m_Impl->start(_id, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on start()" << std::endl;
			throw;
		}
	}

	void Engine::startAll()
	{
		try
		{
			m_Impl->startAll(*m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on startAll()" << std::endl;
			throw;
		}
	}

	void Engine::stop(Identifier const& _id)
	{
		try
		{
			m_Impl->stop(_id, *m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on stop()" << std::endl;
			throw;
		}
	}

	void Engine::stopAll()
	{
		try
		{
			m_Impl->stopAll(*m_ID);
		}
		catch (...)
		{
			std::cout << "engine: error on staopAll()" << std::endl;
			throw;
		}
	}

	void Engine::destroy(Identifier const& _id)
	{
		try
		{
			//m_Impl->destroy(_id);
		}
		catch (...)
		{
			std::cout << "engine: error on destroy()" << std::endl;
			throw;
		}
	}

	void Engine::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src)
	{
		try
		{
			//m_Impl->destroy(_id);
		}
		catch (...)
		{
			std::cout << "engine: error on insert()" << std::endl;
			throw;
		}
	}
}