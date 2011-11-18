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
#include "_2RealEntityTable.h"
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
			try
			{
				m_Engine.entities().destroy(it->second);
			}
			catch (Exception &e)
			{
				std::cout << "error on production graphs destruction: " << e.what() << std::endl;
			}
		}
	}

	const unsigned int ProductionGraphs::createSystemGraph(std::string const& _name)
	{
		SystemGraph *nirvana = m_Engine.entities().createSystem(_name);
		m_Systems.insert(NamedSystem(nirvana->id(), nirvana));
		return nirvana->id();
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
		m_Engine.entities().destroy(nirvana);
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

	const unsigned int ProductionGraphs::createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
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
		return 0;
	}

	const unsigned int ProductionGraphs::createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
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
		return 0;
	}

	Runnable *const ProductionGraphs::belongsToSystem(Identifier const& _system, Identifier const& _runnable) const
	{
		SystemGraph const* nirvana = getSystemGraph(_system);
		std::list< Runnable * > children = nirvana->children();

		for (std::list< Runnable * >::const_iterator it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->id() == _runnable.id())
			{
				return *it;
			}
			// ~~has parameters
			else if (!(*it)->hasParameters())
			{
				RunnableGraph *graph = static_cast< RunnableGraph * >(*it);
				Runnable *child = NULL;
				if ((child = graph->findChild(_runnable.id())))
				{
					return child;
				}
			}
		}

		return NULL;
	}
}