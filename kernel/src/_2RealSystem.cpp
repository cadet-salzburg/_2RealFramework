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
#include "_2RealSystem.h"
#include "_2RealException.h"
#include "_2RealIdentifier.h"
#include "_2RealHelpers.h"
#include "_2RealSystemData.h"

#include <sstream>

#ifdef _2REAL_WINDOWS
	#ifndef _DEBUG
		#define shared_library_suffix ".dll"
	#else
		#define shared_library_suffix "_d.dll"
	#endif
#elif _2REAL_UNIX
	#ifndef _DEBUG
		#define shared_library_suffix ".so"
	#else
		#define shared_library_suffix "_d.so"
	#endif
#elif _2REAL_MAC
	#ifndef _DEBUG
		#define shared_library_suffix ".dylib"
	#else
		#define shared_library_suffix "_d.dylib"
	#endif
#endif

namespace _2Real
{

	System::System(std::string const& name) :
		m_EngineImpl(EngineImpl::instance())
	{
		m_Id = m_EngineImpl.createSystem(name);
	}

	void System::initFromXML(std::string const& configFile)
	{
		try
		{
			SystemData data(configFile);

			Poco::Path pluginPath = data.getDefaultDirectory();
			Poco::Path logPath = data.getLogfile();

			std::list< PluginData > plugins = data.getPlugins();

			for (std::list< PluginData >::iterator it = plugins.begin(); it != plugins.end(); ++it)
			{
				std::string name = it->getName().toString();
				std::string classname = it->getClassname();
				Poco::Path file = pluginPath;
				file.append(it->getPluginPath());
				file.append(classname+shared_library_suffix);

				std::cout << file.toString() << std::endl;

				//m_EngineImpl.loadPlugin(name, file, m_Id);

				//so, now i have a lot of plugins all referring to the same whatever
			}

		}
		catch (XMLFormatException &e)
		{
			clear();
			e.rethrow();
		}
		catch (Poco::XML::SAXParseException &e)
		{
			clear();
			std::ostringstream msg;
			msg << e.message();
			throw XMLFormatException(msg.str());
		}
		catch (std::exception &e)
		{
			clear();
			throw XMLFormatException(e.what());
		}
	}

	typedef std::pair< std::string, Identifier > NamedId;

	System::System(System const& src) :
		m_EngineImpl(src.m_EngineImpl),
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

	void System::clear()
	{
		//m_EngineImpl.shutdownSystem(m_Id);
	}

	System::~System()
	{
		m_EngineImpl.destroySystem(m_Id);
	}

	void System::setLogfile(std::string const& file)
	{
		m_EngineImpl.setSystemLogfile(file, m_Id);
	}

	void System::setup(Identifier const& id)
	{
		m_EngineImpl.setup(id, m_Id);
	}

	const Identifier System::createService(std::string const& name, Identifier const& plugin, std::string const& service)
	{
		std::string idName = validateName(name);
		unique(idName);

		const Identifier id = m_EngineImpl.createService(name, plugin, service, m_Id);
		m_Lookup.insert(NamedId(idName, id));
		return id;
	}

	void System::setUpdateRate(Identifier const& runnable, float updatesPerSecond)
	{
		m_EngineImpl.setUpdateRate(runnable, updatesPerSecond, m_Id);
	}

	void System::setValueInternal(Identifier const& id, std::string const& param, EngineData const& value)
	{
		m_EngineImpl.setValue(id, param, value, m_Id);
	}

	//const Identifier System::createSequence(std::string const& name, Identifier const& idA, Identifier const& idB)
	//{
	//	return m_EngineImpl.createSequence(name, idA, idB, m_Id);
	//}

	//const Identifier System::createSynchronization(std::string const& name, Identifier const& idA, Identifier const& idB)
	//{
	//	return m_EngineImpl.createSynchronization(name, idA, idB, m_Id);
	//}

	void System::linkSlots(Identifier const& inService, std::string const& inName, Identifier const& outService, std::string const& outName)
	{
		m_EngineImpl.linkSlots(inService, inName, outService, outName, m_Id);
	}

	void System::registerToException(ExceptionCallback callback)
	{
		m_EngineImpl.registerToException(callback, m_Id);
	}

	void System::unregisterFromException(ExceptionCallback callback)
	{
		m_EngineImpl.unregisterFromException(callback, m_Id);
	}

	void System::registerToException(IExceptionListener &listener)
	{
		m_EngineImpl.registerToException(listener, m_Id);
	}

	void System::unregisterFromException(IExceptionListener &listener)
	{
		m_EngineImpl.unregisterFromException(listener, m_Id);
	}

	void System::registerToStateChange(Identifier const& runnableId, StateChangeCallback callback)
	{
		m_EngineImpl.registerToStateChange(runnableId, callback, m_Id);
	}

	void System::unregisterFromStateChange(Identifier const& runnableId, StateChangeCallback callback)
	{
		m_EngineImpl.unregisterFromStateChange(runnableId, callback, m_Id);
	}

	void System::registerToStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	{
		m_EngineImpl.registerToStateChange(runnableId, listener, m_Id);
	}

	void System::unregisterFromStateChange(Identifier const& runnableId, IStateChangeListener &listener)
	{
		m_EngineImpl.unregisterFromStateChange(runnableId, listener, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_EngineImpl.registerToNewData(service, name, callback, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, DataCallback callback)
	{
		m_EngineImpl.unregisterFromNewData(service, name, callback, m_Id);
	}

	void System::registerToNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	{
		m_EngineImpl.registerToNewData(service, name, listener, m_Id);
	}

	void System::unregisterFromNewData(Identifier const& service, std::string const& name, IOutputListener &listener)
	{
		m_EngineImpl.unregisterFromNewData(service, name, listener, m_Id);
	}

	void System::start(Identifier const& id)
	{
		m_EngineImpl.start(id, m_Id);
	}

	void System::startAll()
	{
		m_EngineImpl.startAll(m_Id);
	}

	void System::stop(Identifier const& id)
	{
		m_EngineImpl.stop(id, m_Id);
	}

	void System::stopAll()
	{
		m_EngineImpl.stopAll(m_Id);
	}

	//void System::add(Identifier const& runnable, Identifier const& parent, unsigned int index)
	//{
	//	m_EngineImpl.add(runnable, parent, index, m_Id);
	//}

	//void System::append(Identifier const& runnable, Identifier const& parent)
	//{
	//	m_EngineImpl.append(runnable, parent, m_Id);
	//}

	const EngineData System::getValueInternal(Identifier const& id, std::string const& name) const
	{
		return m_EngineImpl.getValue(id, name, m_Id);
	}
}