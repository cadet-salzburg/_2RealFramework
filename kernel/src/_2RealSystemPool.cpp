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

#include "_2RealSystemPool.h"
#include "_2RealEngine.h"
#include "_2RealSystemGraph.h"
#include "_2RealTypetable.h"

#include <sstream>

namespace _2Real
{

	SystemPool::SystemPool(Engine const& engine) :
		m_Engine(engine),	//do not touch anywhere else in ctor
		m_Systems()
	{
	}

	SystemPool::~SystemPool()
	{
	}

	void SystemPool::clearSystems()
	{
		std::ostringstream msg;
		bool error = false;

		for (SystemGraphTable::iterator it = m_Systems.begin(); it != m_Systems.end(); ++it)
		{
			Identifier id = it->first;

			try
			{
				it->second->shutdown();
			}
			catch (std::exception &e)
			{
				error = true;
				msg << "exception on production graph shutdown: " << id << " " << e.what() << std::endl;
			}

			delete it->second;
		}

		m_Systems.clear();

		if (error)
		{
			throw Exception(msg.str());
		}
	}

	const Identifier SystemPool::createSystemGraph(std::string const& name)
	{
		const Identifier id = Entity::createIdentifier(name, "system");
		SystemGraph *graph = new SystemGraph(id);
		m_Systems.insert(NamedSystemGraph(id, graph));
		return id;
	}

	void SystemPool::destroySystemGraph(Identifier const& id)
	{
		SystemGraphTable::iterator it = m_Systems.find(id);

		if (it != m_Systems.end())
		{
			SystemGraph *nirvana = it->second;
			nirvana->shutdown();
			delete nirvana;
			m_Systems.erase(it);
		}
	}

	bool SystemPool::contains(Identifier const& id) const
	{
		return m_Systems.find(id) != m_Systems.end();
	}

	SystemGraph & SystemPool::getSystemGraph(Identifier const& id)
	{
		SystemGraphTable::iterator it = m_Systems.find(id);

		if (it == m_Systems.end())
		{
			std::ostringstream msg;
			msg << "internal error: system " << id.name() << " not found in systems";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}

	SystemGraph const& SystemPool::getSystemGraph(Identifier const& id) const
	{
		SystemGraphTable::const_iterator it = m_Systems.find(id);

		if (it == m_Systems.end())
		{
			std::ostringstream msg;
			msg << "internal error: system " << id.name() << " not found in systems";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}

	//void SystemPool::destroyRunnable(unsigned int const& _id, unsigned int const& _top)
	//{
	//	//Container *nirvana = getSystem(_top);
	//	//AbstractContainer *container = nirvana->get(_id);
	//	//container->shutdown();
	//	//m_Engine.entities().destroy(container);
	//}

	//const Identifier SystemPool::createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	//{
	//	//Container *nirvana = getSystem(_top);
	//	//AbstractContainer *a = nirvana->get(_a);
	//	//AbstractContainer *b = nirvana->get(_b);

	//	//Container *seq = m_Engine.entities().createSequence(_name);
	//	//	
	//	////move sequence into nirvana
	//	//nirvana->add(seq, 0);

	//	////add containers to newly created sequence
	//	//seq->add(b, 0);
	//	//seq->add(a, 0);

	//	//return seq->id();
	//	return Entity::createIdentifier(_name, "sequence");
	//}

	//const Identifier SystemPool::createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	//{
	//	//Container *nirvana = getSystem(_top);
	//	//AbstractContainer *a = nirvana->get(_a);
	//	//AbstractContainer *b = nirvana->get(_b);

	//	//Container *sync =m_Engine.entities().createSynchronization(_name);

	//	////move sync into nirvana
	//	//nirvana->add(sync, 0);

	//	////add to containers to newly created synchronization
	//	//sync->add(a, 0);
	//	//sync->add(b, 1);

	//	//return sync->id();
	//	return Entity::createIdentifier(_name, "synchronization");
	//}

	//Runnable *const SystemPool::belongsToSystem(Identifier const& system, Identifier const& runnable) const
	//{
	//	SystemGraph const* nirvana = getSystemGraph(system);
	//	std::list< Runnable * > children = nirvana->children();

	//	for (std::list< Runnable * >::const_iterator it = children.begin(); it != children.end(); it++)
	//	{
	//		if ((*it)->id() == runnable.id())
	//		{
	//			return *it;
	//		}
	//		// ~~has parameters
	//		else if (!((*it)->type() == "service"))
	//		{
	//			RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
	//			Runnable *child = NULL;
	//			if ((child = graph->findChild(runnable)))
	//			{
	//				return child;
	//			}
	//		}
	//	}

	//	return NULL;
	//}
}