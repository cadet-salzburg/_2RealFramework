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

#include "_2RealImagebuffer.h"

#include "_2RealTypes.h"

#include "_2RealPluginMetadata.h"
#include "_2RealServiceMetadata.h"
#include "_2RealPluginPool.h"
#include "_2RealServiceFactory.h"
#include "_2RealEntityTable.h"
#include "_2RealProductionGraphs.h"
#include "_2RealEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealException.h"
#include "_2RealPlugin.h"
#include "_2RealInputSlot.h"
#include "_2RealOutputSlot.h"
#include "_2RealSetupParameter.h"
#include "_2RealService.h"
#include "_2RealSystemGraph.h"

#include "_2RealSingletonHolder.h"

#include <sstream>
#include <iostream>

namespace _2Real
{

	Engine *const Engine::instance()
	{
		static SingletonHolder< Engine > holder;
		return holder.instance();
	}

	Engine::Engine()
	{
		m_Types = new TypeTable(*this);
		m_Types->registerType< char >("char");
		m_Types->registerType< unsigned char >("unsigned char");
		m_Types->registerType< short >("short");
		m_Types->registerType< unsigned short >("unsigned short");
		m_Types->registerType< int >("int");
		m_Types->registerType< unsigned int >("unsigned int");
		m_Types->registerType< long >("long");
		m_Types->registerType< unsigned long >("unsigned long");
		m_Types->registerType< float >("float");
		m_Types->registerType< double >("double");
		m_Types->registerType< bool >("bool");
		m_Types->registerType< std::string >("string");
		m_Types->registerType< Buffer2D_float >("buffer2D_float");
		m_Types->registerType< Buffer2D_double >("buffer2D_double");
		m_Types->registerType< Buffer2D_uint >("buffer2D_uint");
		m_Types->registerType< Buffer2D_uchar >("buffer2D_uchar");

		m_Entities = new EntityTable();
		m_Plugins = new PluginPool(*this);
		m_Factory = new ServiceFactory(*this);
		m_Graphs = new ProductionGraphs(*this);

		//m_Time = new EngineTime();
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
		delete m_Entities;
		delete m_Types;
		
		//delete m_Time;

		std::cout << "ENGINE: goodbye" << std::endl;
	}

	const Identifier Engine::createSystem(std::string const& _name)
	{
		return m_Graphs->createSystemGraph(_name);
	}

	void Engine::destroySystem(Identifier const& _id)
	{
		m_Graphs->destroySystemGraph(_id);
	}

	const Identifier Engine::installPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, Identifier const& _system)
	{
		return m_Plugins->install(_name, _dir, _file, _class);
	}

	void Engine::startPlugin(Identifier const& _plugin, Identifier const& _system)
	{
		Plugin *plugin = m_Plugins->plugin(_plugin);
		plugin->setup();
	}

	void Engine::dumpPluginInfo(Identifier const& _plugin, Identifier const& _system) const
	{
		PluginMetadata data = m_Plugins->plugin(_plugin)->pluginMetadata();

		std::string info = data.info();
		std::cout << info << std::endl;
	}

	void Engine::dumpServiceInfo(Identifier const& _id, std::string const& _name, Identifier const& _system) const
	{
		ServiceMetadata data = m_Plugins->plugin(_id)->serviceMetadata(_name);

		std::string info = data.info();
		std::cout << info << std::endl;
	}

	const Identifier Engine::createService(std::string const& _name, Identifier const& _id, std::string const& _service, Identifier const& _system)
	{
		Plugin *const plugin = m_Plugins->plugin(_id);
		return m_Factory->createService(_name, plugin, _service, m_Graphs->getSystemGraph(_system));
	}

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

	void Engine::setParameterValue(Identifier const& _id, std::string const& _param, SharedAny _value, std::string const& _type, Identifier const& _system)
	{
		if (m_Plugins->contains(_id))
		{
			Plugin *plugin = m_Plugins->plugin(_id);
			SetupParameter *param = plugin->getSetupParameter(_param);
			if (param->datatype() != _type)
			{
				//BadDatatypeException
				throw Exception("datatype mismatch: " + param->datatype() + " " + _type);
			}
			param->set(_value);
		}
		else
		{
			Runnable *runnable = NULL;
			if ((runnable = m_Graphs->belongsToSystem(_system, _id)) != NULL)
			{
				if (runnable->hasParameters())
				{
					Service *service = static_cast< Service * >(runnable);
					SetupParameter *param = service->getSetupParameter(_param);
					if (param)
					{
						if (param->datatype() != _type)
						{
							//BadDatatypeException
							throw Exception("datatype mismatch: " + param->datatype() + " " + _type);
						}

						param->set(_value);
					}
					else
					{
						//InvalidParameterException
						//_id is a service belongin to _system, but has no setup param named like that
					}
				}
				else
				{
					//InvalidEntityException
					//_id really belongs to _system, but is not a service
				}
			}
			else
			{
				//InvalidEntityException
				//_id is not a runnable belonging to system
			}
		}
	}

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

	void Engine::linkSlots(Identifier const& _idIn, std::string const& _nameIn, Identifier const& _idOut, std::string const& _nameOut, Identifier const& _system)
	{
		Runnable *runnable = NULL;
		InputSlot *in = NULL;
		OutputSlot *out = NULL;

		if ((runnable = m_Graphs->belongsToSystem(_system, _idIn)) != NULL)
		{
			if (runnable->hasParameters())
			{
				Service *serviceIn = static_cast< Service * >(runnable);
				in = serviceIn->getInputSlot(_nameIn);
				if (!in)
				{
					//InvalidParameterException
					//_id is a service belongin to _system, but has no setup param named like that
				}
			}
			else
			{
				//InvalidEntityException
				//_id really belongs to _system, but is not a service
			}
		}
		else
		{
			//InvalidEntityException
			//_id is not a runnable belonging to system
		}

		runnable = NULL;
		if ((runnable = m_Graphs->belongsToSystem(_system, _idOut)) != NULL)
		{
			if (runnable->hasParameters())
			{
				Service *serviceOut = static_cast< Service * >(runnable);
				out = serviceOut->getOutputSlot(_nameOut);
				if (!out)
				{
					//InvalidParameterException
					//_id is a service belongin to _system, but has no setup param named like that
				}
			}
			else
			{
				//InvalidEntityException
				//_id really belongs to _system, but is not a service
			}
		}
		else
		{
			//InvalidEntityException
			//_id is not a runnable belonging to system
		}

		out->addListener(in);
	}

	//void Engine::registerToException(Identifier const& _id, ExceptionCallback _callback, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *container = nirvana->find(_id.id());
	//		if (!container)
	//		{
	//			throw Exception("this system does not contain " + _id.name());
	//		}

	//		container->registerExceptionCallback(_callback);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void Engine::registerToNewData(Identifier const& _service, std::string const& _name, DataCallback _callback, Identifier const& _system)
	{
		Runnable *runnable = NULL;
		if ((runnable = m_Graphs->belongsToSystem(_system, _service)))
		{
			if (runnable->hasParameters())
			{
					Service *service = static_cast< Service * >(runnable);
					OutputSlot *slot = service->getOutputSlot(_name);
					slot->registerCallback(_callback);
			}
			else
			{
				//
			}
		}
		else
		{
		}
	}

	//void Engine::unregisterFromNewData(Identifier const& _service, std::string const& _out, DataCallback _callback, Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		AbstractContainer *child = nirvana->find(_service.id());
	//		Service *service = static_cast< Service * >(child);
	//		OutputSlot *out = service->getOutputSlotByName(_out);

	//		out->unregisterCallback(_callback);
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

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

	void Engine::start(Identifier const& _id, Identifier const& _system)
	{
		SystemGraph *nirvana = m_Graphs->getSystemGraph(_system);
		nirvana->startChild(_id.id());
	}

	//void Engine::startAll(Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		IDs children = nirvana->childIDs();
	//		for (IDIterator it = children.begin(); it != children.end(); it++)
	//		{
	//			nirvana->startChild(*it);
	//		}
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

	void Engine::stop(Identifier const& _id, Identifier const& _system)
	{
		SystemGraph *nirvana = m_Graphs->getSystemGraph(_system);
		nirvana->stopChild(_id.id());
	}

	//void Engine::ssystemAll(Identifier const& _system)
	//{
	//	try
	//	{
	//		Container *nirvana = m_Graphs->getSystem(_system.id());
	//		IDs children = nirvana->childIDs();
	//		for (IDIterator it = children.begin(); it != children.end(); it++)
	//		{
	//			nirvana->ssystemChild(*it);
	//		}
	//	}
	//	catch (Exception &e)
	//	{
	//		throw e;
	//	}
	//}

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
}