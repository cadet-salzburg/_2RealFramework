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
#include "_2RealSystem.h"
#include "_2RealException.h"
#include "_2RealIdentifier.h"

#include <iostream>

namespace _2Real
{

	System::System(std::string const& _name) : m_Engine(NULL), m_ID(NULL)
	{
		try
		{
			m_Engine = Engine::instance();
			m_ID = new Identifier(m_Engine->createSystem(_name));
		}
		catch (Exception &e)
		{

			if (m_Engine)
			{
				m_Engine->release();
				m_Engine = NULL;
			}

			delete m_ID;

			throw e;
		}
	}

	System::System(System const& _src) : m_Engine(NULL), m_ID(NULL)
	{
		m_Engine = Engine::instance();
		
		if (_src.m_ID)
		{
			m_ID = new Identifier(*_src.m_ID);
		}
	}
	
	System& System::operator=(System const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		if (!m_Engine)
		{
			m_Engine = Engine::instance();
		}

		if (m_ID)
		{
			//this will delete everything inside of the graph
			m_Engine->destroySystem(*m_ID);
		}

		if (_src.m_ID)
		{
			m_ID = new Identifier(*_src.m_ID);
		}

		return *this;
	}

	System::~System()
	{
		m_Engine->destroySystem(*m_ID);
		delete m_ID;
		m_Engine->release();
	}

	const Identifier System::getID()
	{
		return *m_ID;
	}

	const Identifier System::loadPlugin(std::string const& _name, std::string const& _directory, std::string const& _file, std::string const& _classname)
	{
		try
		{
			return m_Engine->installPlugin(_name, _directory, _file, _classname, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::dumpPluginInfo(Identifier const& _pluginID)
	{
		try
		{
			m_Engine->dumpPluginInfo(_pluginID, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier System::createService(std::string const& _name, Identifier const& _pluginID, std::string const& _serviceName)
	{
		try
		{
			return m_Engine->createService(_name, _pluginID, _serviceName, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::dumpServiceInfo(Identifier const& _pluginID, std::string const& _serviceName)
	{
		try
		{
			m_Engine->dumpServiceInfo(_pluginID, _serviceName, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers System::getSetupParameters(Identifier const& _id)
	{
		try
		{
			//return m_Engine->getSetupParameters(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers System::getInputSlots(Identifier const& _id)
	{
		try
		{
			//return m_Engine->getInputSlots(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers System::getOutputSlots(Identifier const& _id)
	{
		try
		{
			//return m_Engine->getOutputSlots(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier System::createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	{
		try
		{
			//return m_Engine->createSequence(_name, _idA, _idB, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const Identifier System::createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	{
		try
		{
			//return m_Engine->createSynchronization(_name, _idA, _idB, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::setParameterValue(Identifier const& _id, Poco::Any _any)
	{
		try
		{
			//m_Engine->setParameterValue(_id, _any, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::link(Identifier const& _in, Identifier const& _out)
	{
		try
		{
			//m_Engine->link(_in, _out, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::registerToException(Identifier const& _id, ExceptionCallback _callback)
	{
		try
		{
			//m_Engine->registerToException(_id, _callback, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::registerToNewData(Identifier const& _id, NewDataCallback _callback)
	{
		try
		{
			//m_Engine->registerToNewData(_id, _callback, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Identifiers System::getChildren(Identifier const& _id)
	{
		try
		{
			//return m_Engine->getChildren(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::start(Identifier const& _id)
	{
		try
		{
			//m_Engine->start(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::startAll()
	{
		try
		{
			//m_Engine->startAll(*m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::stop(Identifier const& _id)
	{
		try
		{
			//m_Engine->stop(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::stopAll()
	{
		try
		{
			//m_Engine->stopAll(*m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::destroy(Identifier const& _id)
	{
		try
		{
			//m_Engine->destroy(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void System::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src)
	{
		try
		{
			//m_Engine->destroy(_id, *m_ID);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}