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
		m_Graphs(*this),
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
			m_Graphs.clearSystems();
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

	const Identifier Engine::load(std::string const& name, std::string const& classname, Identifier const& systemId)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(systemId);

		return nirvana.install(name, classname);
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

		nirvana.setup(setupAble);
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
			msg << "engine::getInfo " << id.name() << " is a " << id.type() << ", service expected" << std::endl;
			throw InvalidIdentifierException(msg.str());
		}

		nirvana.setUpdateRate(id, updatesPerSecond);
	}

	void Engine::linkSlots(Identifier const& idIn, std::string const& nameIn, Identifier const& idOut, std::string const& nameOut, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		//if (nirvana.contains(idIn) && nirvana.contains(idOut))
		//{
		//	Runnable &childIn = nirvana.getChild(idIn);
		//	Runnable &childOut = nirvana.getChild(idOut);
		//	if (childIn.type() == "service" && childOut.type() == "service")
		//	{
		//		Service &serviceIn = static_cast< Service & >(childIn);
		//		Service &serviceOut = static_cast< Service & >(childOut);

		//		if (serviceIn.hasInputSlot(nameIn) && serviceOut.hasOutputSlot(nameOut))
		//		{
		//			InputSlot &in = serviceIn.getInputSlot(nameIn);
		//			OutputSlot &out = serviceOut.getOutputSlot(nameOut);

		//			out.addListener(in);
		//			in.linkWith(out);
		//		}
		//		else
		//		{
		//		}
		//	}
		//	else
		//	{
		//	}
		//}
		//else
		//{
		//}
	}

	void Engine::registerToException(ExceptionCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.registerExceptionCallback(callback);
	}

	void Engine::unregisterFromException(ExceptionCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.unregisterExceptionCallback(callback);
	}

	void Engine::registerToNewData(Identifier const& id, std::string const& name, DataCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		//if (nirvana.contains(id))
		//{
		//	Runnable &child = nirvana.getChild(id);
		//	if (child.type() == "service")
		//	{
		//		Service &service = static_cast< Service & >(child);
		//		OutputSlot &out = service.getOutputSlot(name);
		//		out.registerCallback(callback);
		//	}
		//	else
		//	{
		//	}
		//}
		//else
		//{
		//}
	}

	void Engine::unregisterFromNewData(Identifier const& id, std::string const& name, DataCallback callback, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		//if (nirvana.contains(id))
		//{
		//	Runnable &child = nirvana.getChild(id);
		//	if (child.type() == "service")
		//	{
		//		Service &service = static_cast< Service & >(child);
		//		OutputSlot &out = service.getOutputSlot(name);
		//		out.unregisterCallback(callback);
		//	}
		//	else
		//	{
		//	}
		//}
		//else
		//{
		//}
	}

	void Engine::registerToNewData(Identifier const& id, std::string const& name, OutputListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		//if (nirvana.contains(id))
		//{
		//	Runnable &child = nirvana.getChild(id);
		//	if (child.type() == "service")
		//	{
		//		Service &service = static_cast< Service & >(child);
		//		OutputSlot &out = service.getOutputSlot(name);
		//		out.addListener(listener);
		//	}
		//	else
		//	{
		//	}
		//}
		//else
		//{
		//}
	}

	void Engine::unregisterFromNewData(Identifier const& id, std::string const& name, OutputListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		//if (nirvana.contains(id))
		//{
		//	Runnable &child = nirvana.getChild(id);
		//	if (child.type() == "service")
		//	{
		//		Service &service = static_cast< Service & >(child);
		//		OutputSlot &out = service.getOutputSlot(name);
		//		out.removeListener(listener);
		//	}
		//	else
		//	{
		//	}
		//}
		//else
		//{
		//}
	}

	void Engine::registerToException(ExceptionListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.registerExceptionListener(listener);
	}

	void Engine::unregisterFromException(ExceptionListener &listener, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.unregisterExceptionListener(listener);
	}

	void Engine::start(Identifier const& id, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.startChild(id);
	}

	void Engine::stop(Identifier const& id, Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.stopChild(id);
	}

	////DataHandle Engine::createDataHandle(Identifier const& _service, std::string const& _out, Identifier const& _system)
	////{
	////	try
	////	{
	////		Container *nirvana = m_Graphs->getSystem(_system.id());
	////		AbstractContainer *child = nirvana->find(_service.id());
	////		Service *service = static_cast< Service * >(child);
	////		OutputSlot *out = service->getOutputSlotByName(_out);
	////		
	////		return out->createHandle();
	////	}
	////	catch (Exception &e)
	////	{
	////		throw e;
	////	}
	////}

	//Identifiers Engine::getChildren(Identifier const& _id, Identifier const& _system)
	//{
	//	try
	//	{
	//		IDs ids;
	//		Identifiers children;
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		if (_id == _system)
	//		{
	//			ids = nirvana->childIDs();
	//		}
	//		else
	//		{
	//			const AbstractContainer *container = nirvana->find(_id.id());
	//			if (!container)
	//			{
	//				throw Exception("this system does not contain " + _id.name());
	//			}
	//			else if (container->type() == Entity::SERVICE)
	//			{
	//				throw Exception("services do not have children");
	//			}

	//			const Container *c = static_cast< const Container * >(container);
	//			ids = c->childIDs();
	//		}

	//		for (IDIterator it = ids.begin(); it != ids.end(); it++)
	//		{
	//			children.push_back(m_Entities->getIdentifier(*it));
	//		}
	//		return children;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void Engine::startAll(Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.startAll();
	}

	void Engine::stopAll(Identifier const& system)
	{
		SystemGraph &nirvana = m_Graphs.getSystemGraph(system);

		nirvana.stopAll();
	}

	//void Engine::insert(Identifier const& _dst, unsigned int const& _index, Identifier const& _src, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *src = nirvana->get(_src.id());
	//		if (!src)
	//		{
	//			throw Exception("this system does not contain " + _src.name());
	//		}
	//		if (_dst.id() != _system.id())
	//		{
	//			AbstractContainer *dst = nirvana->find(_dst.id());
	//			if (!dst)
	//			{
	//				throw Exception("this system does not contain " + _dst.name());
	//			}
	//			Container *container = static_cast< Container * >(dst);
	//			container->add(src, _index);
	//		}
	//		else
	//		{
	//			nirvana->add(src, _index);
	//		}
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void Engine::append(Identifier const& _dst, Identifier const& _src, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *src = nirvana->get(_src.id());
	//		if (!src)
	//		{
	//			throw Exception("this system does not contain " + _src.name());
	//		}
	//		if (_dst.id() != _system.id())
	//		{
	//			AbstractContainer *dst = nirvana->find(_dst.id());
	//			if (!dst)
	//			{
	//				throw Exception("this system does not contain " + _dst.name());
	//			}
	//			Container *container = static_cast< Container * >(dst);
	//			container->add(src, container->childCount());
	//		}
	//		else
	//		{
	//			nirvana->add(src, nirvana->childCount());
	//		}
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void Engine::destroy(Identifier const& _id, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *container = nirvana->get(_id.id());
	//		if (!container)
	//		{
	//			throw Exception("this system does not contain " + _id.name());
	//		}
	//		container->shutdown();
	//		m_Entities->destroy(container);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

		//Identifiers Engine::getSetupParameters(Identifier const& _id, Identifier const& _system) const
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		const AbstractContainer *container = nirvana->find(_id.id());
	//		const Plugin *plugin = m_Plugins->plugin(_id.id());
	//		if (!container && !plugin)
	//		{
	//			throw Exception("this system does not contain " + _id.name());
	//		}
	//		else if (container && container->type() != Entity::SERVICE)
	//		{
	//			throw Exception("only setup params of a service can be queried");
	//		}
	//		Identifiers setup;
	//		IDs ids;

	//		if (container)
	//		{
	//			ids = container->setupParameterIDs();
	//		}
	//		else
	//		{
	//			ids = plugin->setupParameterIDs();
	//		}
	//		for (IDIterator it = ids.begin(); it != ids.end(); it++)
	//		{
	//			setup.push_back(m_Entities->getIdentifier(*it));
	//		}
	//		return setup;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers Engine::getInputSlots(Identifier const& _id, Identifier const& _system) const
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		const AbstractContainer *container = nirvana->find(_id.id());

	//		if (!container)
	//		{
	//			throw Exception("this system does not contain " + _id.name());
	//		}

	//		Identifiers input;
	//		IDs ids = container->inputSlotIDs();
	//		for (IDIterator it = ids.begin(); it != ids.end(); it++)
	//		{
	//			input.push_back(m_Entities->getIdentifier(*it));
	//		}

	//		return input;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//Identifiers Engine::getOutputSlots(Identifier const& _id, Identifier const& _system) const
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		const AbstractContainer *container = nirvana->find(_id.id());

	//		if (!container)
	//		{
	//			throw Exception("this system does not contain " + _id.name());
	//		}

	//		Identifiers output;
	//		IDs ids = container->outputSlotIDs();
	//		for (IDIterator it = ids.begin(); it != ids.end(); it++)
	//		{
	//			output.push_back(m_Entities->getIdentifier(*it));
	//		}

	//		return output;
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier Engine::createSequence(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system)
	//{
	//	try
	//	{
	//		unsigned int id = m_Graphs->createSequence(_name, _idA.id(), _idB.id(), _system.id());
	//		return m_Entities->getIdentifier(id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//const Identifier Engine::createSynchronization(std::string const& _name, Identifier const& _idA, Identifier const& _idB, Identifier const& _system)
	//{
	//	try
	//	{
	//		unsigned int id = m_Graphs->createSynchronization(_name, _idA.id(), _idB.id(), _system.id());
	//		return m_Entities->getIdentifier(id);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	//void Engine::link(Identifier const& _in,  Identifier const& _out, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *in = nirvana->find(_in.id());
	//		if (!in)
	//		{
	//			throw Exception("this system does not contain " + _in.name());
	//		}
	//		else if (in->father() != nirvana)
	//		{
	//			throw Exception(_in.name() + " is not a child of system");
	//		}
	//		AbstractContainer *out = nirvana->find(_out.id());
	//		if (!out)
	//		{
	//			throw Exception("this system does not contain " + _out.name());
	//		}
	//		else if (out->father() != nirvana)
	//		{
	//			throw Exception(_out.name() + " is not a child of system");
	//		}
	//		if (in == out)
	//		{
	//			throw Exception(_in.name() + " is identical to " + _out.name());
	//		}

	//		std::list< InputSlot * > inSlots = in->inputSlots();
	//		std::list< OutputSlot * > outSlots = out->outputSlots();
	//		std::list< InputSlot * >::iterator inIt;
	//		std::list< OutputSlot * >::iterator outIt;

	//		if (inSlots.size() != outSlots.size())
	//		{
	//			throw Exception("can not link " + _in.name() + " " + _out.name() + " , IO mismatch");
	//		}

	//		nirvana->ssystemChild(_in.id());
	//		nirvana->ssystemChild(_out.id());
	//		for (inIt = inSlots.begin(), outIt = outSlots.begin(); inIt != inSlots.end(), outIt != outSlots.end(); inIt++, outIt++)
	//		{
	//			//(*outIt)->linkWith(*inIt);
	//		}
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

}