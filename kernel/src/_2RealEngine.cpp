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

}