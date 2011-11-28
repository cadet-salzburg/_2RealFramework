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

#include "_2RealProductionGraphs.h"
#include "_2RealSystemGraph.h"
#include "_2RealRunnable.h"
#include "_2RealRunnableGraph.h"
//#include "_2RealServiceTemplate.h"

#include <iostream>

namespace _2Real
{
	ProductionGraphs::ProductionGraphs(Engine &_engine) :
		m_Systems(),
		m_Engine(_engine)
	{
	}

	ProductionGraphs::~ProductionGraphs()
	{
		for (SystemTable::iterator it = m_Systems.begin(); it != m_Systems.end(); it++)
		{
			std::string name;
			try
			{
				name = it->second->name();
				std::cout << "deleting system: " << name << std::endl;
				delete it->second;
			}
			catch (Exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (...)
			{
				std::cout << "error on system destruction: " << name << std::endl;
			}
		}
	}

	const Identifier ProductionGraphs::createSystemGraph(std::string const& _name)
	{
		const Identifier id = Entity::createIdentifier(_name, "system");
		SystemGraph *graph = new SystemGraph(id);
		m_Systems.insert(NamedSystem(id.id(), graph));
		return id;
	}

	void ProductionGraphs::destroySystemGraph(Identifier const& _id)
	{
		SystemTable::iterator it = m_Systems.find(_id.id());

		if (it == m_Systems.end())
		{
			throw Exception("internal error, attempted to destroy a system which does not exist");
		}

		SystemGraph *nirvana = it->second;
		nirvana->stopAll();

		//deletes all children
		delete nirvana;
		m_Systems.erase(it);
	}

	const bool ProductionGraphs::isSystemGraph(Identifier const& _id)
	{
		SystemTable::const_iterator it = m_Systems.find(_id.id());
		return it != m_Systems.end();
	}

	SystemGraph *const ProductionGraphs::getSystemGraph(Identifier const& _id)
	{
		SystemTable::iterator it = m_Systems.find(_id.id());

		if (it == m_Systems.end())
		{
			throw Exception("internal error, tried to access nonexistant system");
		}

		return it->second;
	}

	SystemGraph const *const ProductionGraphs::getSystemGraph(Identifier const& _id) const
	{
		SystemTable::const_iterator it = m_Systems.find(_id.id());

		if (it == m_Systems.end())
		{
			throw Exception("internal error, tried to access nonexistant system");
		}

		return it->second;
	}

	void ProductionGraphs::destroyRunnable(unsigned int const& _id, unsigned int const& _top)
	{
		//Container *nirvana = getSystem(_top);
		//AbstractContainer *container = nirvana->get(_id);
		//container->shutdown();
		//m_Engine.entities().destroy(container);
	}

	const Identifier ProductionGraphs::createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		//Container *nirvana = getSystem(_top);
		//AbstractContainer *a = nirvana->get(_a);
		//AbstractContainer *b = nirvana->get(_b);

		//Container *seq = m_Engine.entities().createSequence(_name);
		//	
		////move sequence into nirvana
		//nirvana->add(seq, 0);

		////add containers to newly created sequence
		//seq->add(b, 0);
		//seq->add(a, 0);

		//return seq->id();
		return Entity::createIdentifier(_name, "sequence");
	}

	const Identifier ProductionGraphs::createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		//Container *nirvana = getSystem(_top);
		//AbstractContainer *a = nirvana->get(_a);
		//AbstractContainer *b = nirvana->get(_b);

		//Container *sync =m_Engine.entities().createSynchronization(_name);

		////move sync into nirvana
		//nirvana->add(sync, 0);

		////add to containers to newly created synchronization
		//sync->add(a, 0);
		//sync->add(b, 1);

		//return sync->id();
		return Entity::createIdentifier(_name, "synchronization");
	}

	Runnable *const ProductionGraphs::belongsToSystem(Identifier const& system, Identifier const& runnable) const
	{
		SystemGraph const* nirvana = getSystemGraph(system);
		std::list< Runnable * > children = nirvana->children();

		for (std::list< Runnable * >::const_iterator it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->id() == runnable.id())
			{
				return *it;
			}
			// ~~has parameters
			else if (!((*it)->type() == "service"))
			{
				RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
				Runnable *child = NULL;
				if ((child = graph->findChild(runnable)))
				{
					return child;
				}
			}
		}

		return NULL;
	}
}