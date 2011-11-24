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
	}

	const Identifier System::getID()
	{
		return *m_ID;
	}

	const Identifier System::loadPlugin(std::string const& _name, std::string const& _directory, std::string const& _file, std::string const& _classname)
	{
		return m_Engine->installPlugin(_name, _directory, _file, _classname, *m_ID);
	}

	void System::startPlugin(Identifier const& _pluginID)
	{
		return m_Engine->startPlugin(_pluginID, *m_ID);
	}

	void System::dumpPluginInfo(Identifier const& _pluginID)
	{
		m_Engine->dumpPluginInfo(_pluginID, *m_ID);
	}

	void System::dumpServiceInfo(Identifier const& _pluginID, std::string const& _serviceName)
	{
		m_Engine->dumpServiceInfo(_pluginID, _serviceName, *m_ID);
	}

	const Identifier System::createService(std::string const& _name, Identifier const& _pluginID, std::string const& _serviceName)
	{
		return m_Engine->createService(_name, _pluginID, _serviceName, *m_ID);
	}

	void System::setParameterValue(Identifier const& _id, std::string const& _paramName, SharedAny _any, type_info const& _info)
	{
		m_Engine->setParameterValue(_id, _paramName, _any, _info.name(), *m_ID);
	}

	//Identifiers System::getInputSlots(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getInputSlots(_id, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers System::getOutputSlots(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getOutputSlots(_id, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier System::createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	//{
	//	try
	//	{
	//		return m_Engine->createSequence(_name, _idA, _idB, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier System::createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB)
	//{
	//	try
	//	{
	//		return m_Engine->createSynchronization(_name, _idA, _idB, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::link(Identifier const& _in, Identifier const& _out)
	//{
	//	try
	//	{
	//		m_Engine->link(_in, _out, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void System::linkSlots(Identifier const& _idIn, std::string const& _nameIn, Identifier const& _idOut, std::string const& _nameOut)
	{
		m_Engine->linkSlots(_idIn, _nameIn, _idOut, _nameOut, *m_ID);
	}

	void System::registerToException(ExceptionCallback _callback)
	{
		m_Engine->registerToException(_callback, *m_ID);
	}

	void System::unregisterFromException(ExceptionCallback _callback)
	{
		m_Engine->unregisterFromException(_callback, *m_ID);
	}

	void System::registerToNewData(Identifier const& _service, std::string const& _name, DataCallback _callback)
	{
		m_Engine->registerToNewData(_service, _name, _callback, *m_ID);
	}

	void System::unregisterFromNewData(Identifier const& _service, std::string const& _name, DataCallback _callback)
	{
		m_Engine->unregisterFromNewData(_service, _name, _callback, *m_ID);
	}

	//DataHandle System::getDataHandle(Identifier const& _service, std::string const& _out)
	//{
	//	try
	//	{
	//		return m_Engine->createDataHandle(_service, _out, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers System::getChildren(Identifier const& _id)
	//{
	//	try
	//	{
	//		return m_Engine->getChildren(_id, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void System::start(Identifier const& _id)
	{
		m_Engine->start(_id, *m_ID);
	}

	//void System::startAll()
	//{
	//	try
	//	{
	//		m_Engine->startAll(*m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void System::stop(Identifier const& _id)
	{
		m_Engine->stop(_id, *m_ID);
	}

	//void System::stopAll()
	//{
	//	try
	//	{
	//		m_Engine->stopAll(*m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::destroy(Identifier const& _id)
	//{
	//	try
	//	{
	//		m_Engine->destroy(_id, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src)
	//{
	//	try
	//	{
	//		m_Engine->insert(_dst, _index, _src, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void System::append(Identifier const& _dst, Identifier const& _src)
	//{
	//	try
	//	{
	//		m_Engine->append(_dst, _src, *m_ID);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}
}