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
#include "_2RealIdentifier.h"
#include "_2RealSingletonHolder.h"
#include "_2RealSystemGraph.h"

#include "_2RealImagebuffer.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	Engine & Engine::instance()
	{
		static SingletonHolder< Engine > holder;
		return holder.instance();
	}

	Engine::Engine() :
		m_Graphs(),
		m_Factory(*this),
		m_Types(*this),
		m_Timer()
	{
		m_Types.registerType< char >("char");
		m_Types.registerType< unsigned char >("unsigned char");
		m_Types.registerType< short >("short");
		m_Types.registerType< unsigned short >("unsigned short");
		m_Types.registerType< int >("int");
		m_Types.registerType< unsigned int >("unsigned int");
		m_Types.registerType< long >("long");
		m_Types.registerType< unsigned long >("unsigned long");
		m_Types.registerType< float >("float");
		m_Types.registerType< double >("double");
		m_Types.registerType< bool >("bool");
		m_Types.registerType< std::string >("string");
		m_Types.registerType< Buffer2D_float >("buffer2D_float");
		m_Types.registerType< Buffer2D_double >("buffer2D_double");
		m_Types.registerType< Buffer2D_uint >("buffer2D_uint");
		m_Types.registerType< Buffer2D_uchar >("buffer2D_uchar");
	}

	Engine::~Engine()
	{
		try
		{
			m_Graphs.clearSystemGraphs();
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	const Identifier Engine::createSystem(std::string const& systemName)
	{
		return m_Graphs.createSystemGraph(systemName);
	}

	void Engine::destroySystem(Identifier const& systemId)
	{
		m_Graphs.destroySystemGraph(systemId);
	}

	void Engine::setSystemLogfile(std::string const& filepath, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.setLogfile(filepath);
		if (!nirvana.isLoggingEnabled())
		{
			std::cout << "could not start logging into file: " << filepath << std::endl;
		}
	}

	void Engine::setSystemDirectory(std::string const& directory, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);
		
		nirvana.setInstallDirectory(directory);
	}

	const Identifier Engine::loadPlugin(std::string const& name, std::string const& classname, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		return nirvana.loadPlugin(name, classname);
	}

	void Engine::setup(Identifier const& setupAble, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		if (!setupAble.isSetupAble())
		{
			std::ostringstream msg;
			msg << "engine::setup " << setupAble.name() << " is a " << setupAble.type() << ", plugin or service expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.setUp(setupAble);
	}

	const std::string Engine::getInfo(Identifier const& pluginId, Identifier const& systemId) const
	{
		SystemGraph const& nirvana = m_Graphs.getSystemGraph(systemId);

		if (!pluginId.isPlugin())
		{
			std::ostringstream msg;
			msg << "engine::getInfo " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected";
			throw InvalidIdentifierException(msg.str());
		}

		return nirvana.getInfoString(pluginId);
	}

	const Identifier Engine::createService(std::string const& name, Identifier const& pluginId, std::string const& serviceName, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		if (!pluginId.isPlugin())
		{
			std::ostringstream msg;
			msg << "engine::getInfo " << pluginId.name() << " is a " << pluginId.type() << ", plugin expected" << std::endl;
			throw InvalidIdentifierException(msg.str());
		}

		return nirvana.createService(name, pluginId, serviceName);
	}

	void Engine::setValue(Identifier const& id, std::string const& paramName, EngineData const& value, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		if (!id.isSetupAble())
		{
			std::ostringstream msg;
			msg << "engine::setValue " << id.name() << " is a " << id.type() << ", plugin or service expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.setValue(id, paramName, value);
	}

	void Engine::setUpdateRate(Identifier const& id, float updatesPerSecond, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!id.isService())
		{
			std::ostringstream msg;
			msg << "set update rate: " << id.name() << " is a " << id.type() << ", service expected" << std::endl;
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.setUpdateRate(id, updatesPerSecond);
	}

	void Engine::linkSlots(Identifier const& serviceIn, std::string const& nameIn, Identifier const& serviceOut, std::string const& nameOut, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.linkSlots(serviceIn, nameIn, serviceOut, nameOut);
	}

	void Engine::registerToException(ExceptionCallback callback, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.registerExceptionCallback(callback);
	}

	void Engine::unregisterFromException(ExceptionCallback callback, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.unregisterExceptionCallback(callback);
	}

	void Engine::registerToException(IExceptionListener &listener, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.registerExceptionListener(listener);
	}

	void Engine::unregisterFromException(IExceptionListener &listener, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		nirvana.unregisterExceptionListener(listener);
	}

	void Engine::registerToNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!service.isService())
		{
			std::ostringstream msg;
			msg << "registerToNewData: " << service.name() << " is a " << service.type() << ", service expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.registerToNewData(service, outName, callback);
	}

	void Engine::unregisterFromNewData(Identifier const& service, std::string const& outName, DataCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!service.isService())
		{
			std::ostringstream msg;
			msg << "unregisterFromNewData: " << service.name() << " is a " << service.type() << ", service expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.unregisterFromNewData(service, outName, callback);
	}

	void Engine::registerToNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!service.isService())
		{
			std::ostringstream msg;
			msg << "registerToNewData: " << service.name() << " is a " << service.type() << ", service expected";
			throw InvalidIdentifierException(msg.str());
		}
		
		nirvana.registerToNewData(service, outName, listener);
	}

	void Engine::unregisterFromNewData(Identifier const& service, std::string const& outName, IOutputListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!service.isService())
		{
			std::ostringstream msg;
			msg << "unregisterFromNewData: " << service.name() << " is a " << service.type() << ", service expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.unregisterFromNewData(service, outName, listener);
	}

	void Engine::start(Identifier const& runnable, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnable.isRunAble())
		{
			std::ostringstream msg;
			msg << "start: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.startChild(runnable);
	}

	void Engine::stop(Identifier const& runnable, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnable.isRunAble())
		{
			std::ostringstream msg;
			msg << "stop: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.stopChild(runnable);
	}

	void Engine::startAll(Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);
		nirvana.startAll();
	}

	void Engine::stopAll(Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);
		nirvana.stopAll();
	}

	const Identifier Engine::createSequence(std::string const& name, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnableA.isRunAble())
		{
			std::ostringstream msg;
			msg << "createSequence: " << runnableA.name() << " is a " << runnableA.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}
		if (!runnableB.isRunAble())
		{
			std::ostringstream msg;
			msg << "createSequence: " << runnableB.name() << " is a " << runnableB.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}

		return nirvana.createSequence(name, runnableA, runnableB);
	}

	const Identifier Engine::createSynchronization(std::string const& name, Identifier const& runnableA, Identifier const& runnableB, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnableA.isRunAble())
		{
			std::ostringstream msg;
			msg << "createSynchronization: " << runnableA.name() << " is a " << runnableA.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}
		if (!runnableB.isRunAble())
		{
			std::ostringstream msg;
			msg << "createSynchronization: " << runnableB.name() << " is a " << runnableB.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}

		return nirvana.createSynchronization(name, runnableA, runnableB);
	}

	void Engine::add(Identifier const& runnable, Identifier const& parent, unsigned int index, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnable.isRunAble())
		{
			std::ostringstream msg;
			msg << "insert: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}
		if (!parent.isContainer())
		{
			std::ostringstream msg;
			msg << "insert: " << parent.name() << " is a " << parent.type() << ", container (sequence/synchronization/system) expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.add(runnable, parent, index);
	}

	void Engine::append(Identifier const& runnable, Identifier const& parent, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		if (!runnable.isRunAble())
		{
			std::ostringstream msg;
			msg << "append: " << runnable.name() << " is a " << runnable.type() << ", runnable (service/sequence/synchronization) expected";
			throw InvalidIdentifierException(msg.str());
		}
		if (!parent.isContainer())
		{
			std::ostringstream msg;
			msg << "append: " << parent.name() << " is a " << parent.type() << ", container (sequence/synchronization/system) expected";
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.append(runnable, parent);
	}

	//void Engine::destroy(Identifier const& _id, Identifier const& _system)
	//{
	//}

}