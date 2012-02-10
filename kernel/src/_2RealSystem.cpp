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
#include "_2RealHelpers.h"
#include "_2RealSystemData.h"

#include <sstream>

namespace _2Real
{

	System::System(std::string const& name) :
		m_Engine(Engine::instance())
	{
		m_Id = m_Engine.createSystem(name);
	}

	void System::initFromXML(std::string const& configFile)
	{
		try
		{
			SystemData data(configFile);

			setInstallDirectory(data.getDefaultDirectory());
			setLogfile(data.getLogfile());



		}
		catch (XMLFormatException &e)
		{
			shutdown();
			e.rethrow();
		}
		catch (Poco::XML::SAXParseException &e)
		{
			shutdown();
			std::ostringstream msg;
			msg << e.message();
			throw XMLFormatException(msg.str());
		}
		catch (std::exception &e)
		{
			shutdown();
			throw XMLFormatException(e.what());
		}
	}

	typedef std::pair< std::string, Identifier > NamedId;

	System::System(System const& src) :
		m_Engine(src.m_Engine),
		m_Id(src.m_Id),
		m_Lookup()
	{
	}

	void System::unique(std::string const& s) const
	{
		if (m_Lookup.find(s) != m_Lookup.end())
		{
			std::ostringstream msg;
			msg << "name " << s << " already exists!";
			throw AlreadyExistsException(msg.str());
		}
	}

	void System::shutdown()
	{
		//m_Engine.shutdownSystem(m_Id);
	}

	System::~System()
	{
		m_Engine.destroySystem(m_Id);
	}

	void System::setLogfile(std::string const& file)
	{
		m_Engine.setSystemLogfile(file, m_Id);
	}

	void System::setInstallDirectory(std::string const& directory)
	{
		m_Engine.setSystemDirectory(directory, m_Id);
	}

	const Identifier System::loadPlugin(std::string const& name, std::string const& classname, std::string const& path)
	{
		std::string idName = validateName(name);
		unique(idName);

		const Identifier id = m_Engine.loadPlugin(name, classname, m_Id);
		m_Lookup.insert(NamedId(idName, id));
		return id;
	}

	void System::setup(Identifier const& id)
	{
		m_Engine.setup(id, m_Id);
	}

	const std::string System::getInfo(Identifier const& plugin)
	{
		return m_Engine.getInfo(plugin, m_Id);
	}

	const Identifier System::createService(std::string const& name, Identifier const& plugin, std::string const& service)
	{
		std::string idName = validateName(name);
		unique(idName);

		const Identifier id = m_Engine.createService(name, plugin, service, m_Id);
		m_Lookup.insert(NamedId(idName, id));
		return id;
	}

	void System::setUpdateRate(Identifier const& runnable, float updatesPerSecond)
	{
		m_Engine.setUpdateRate(runnable, updatesPerSecond, m_Id);
	}

	void System::setValueInternal(Identifier const& id, std::string const& param, EngineData const& value)
	{
		m_Engine.setValue(id, param, value, m_Id);
	}

	//const Identifier System::createSequence(std::string const& name, Identifier const& idA, Identifier const& idB)
	//{
	//	return m_Engine.createSequence(name, idA, idB, m_Id);
	//}

	//const Identifier System::createSynchronization(std::string const& name, Identifier const& idA, Identifier const& idB)
	//{
	//	return m_Engine.createSynchronization(name, idA, idB, m_Id);
	//}

	void System::linkSlots(Identifier const& inService, std::string const& inName, Identifier const& outService, std::string const& outName)
	{
		m_Engine.linkSlots(inService, inName, outService, outName, m_Id);
	}

	void System::registerToException(ExceptionCallback callback)
	{
		m_Engine.registerToException(callback, m_Id);
	}

	void System::unregisterFromException(ExceptionCallback callback)
	{
		m_Engine.unregisterFromException(callback, m_Id);
	}

	void System::registerToException(IExceptionListener &listener)
	{
		m_Engine.registerToException(listener, m_Id);
	}

	void System::unregisterFromException(IExceptionListener &listener)
	{
		m_Engine.unregisterFromException(listener, m_Id);
	}

	void System::registerToStateChange(Identifier const& runnableId, StateChangeCallback callback)
	{
		m_Engine.registerToStateChange(runnableId, callback, m_Id);
	}

	void System::unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback)
	{
		m_Engine.unregisterFromStateChange(runnableId, callback, m_Id);
	}

	void System::registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	{
		m_Engine.registerToStateChange(runnableId, listener, m_Id);
	}

	void System::unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	{
		m_Engine.unregisterFromStateChange(runnableId, listener, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Engine.registerToNewData(service, name, callback, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_Engine.unregisterFromNewData(service, name, callback, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	{
		m_Engine.registerToNewData(service, name, listener, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	{
		m_Engine.unregisterFromNewData(service, name, listener, m_Id);
	}

	void System::start(Identifier const& id)
	{
		m_Engine.start(id, m_Id);
	}

	void System::startAll()
	{
		m_Engine.startAll(m_Id);
	}

	void System::stop(Identifier const& id)
	{
		m_Engine.stop(id, m_Id);
	}

	void System::stopAll()
	{
		m_Engine.stopAll(m_Id);
	}

	//void System::add(Identifier const& runnable, Identifier const& parent, unsigned int index)
	//{
	//	m_Engine.add(runnable, parent, index, m_Id);
	//}

	//void System::append(Identifier const& runnable, Identifier const& parent)
	//{
	//	m_Engine.append(runnable, parent, m_Id);
	//}

	const EngineData System::getValueInternal(Identifier const& id, std::string const& name) const
	{
		return m_Engine.getValue(id, name, m_Id);
	}
}