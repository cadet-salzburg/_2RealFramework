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
#include "_2RealException.h"
#include "_2RealContainer.h"
#include "_2RealEntityTable.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealServiceContainer.h"

#include <iostream>

namespace _2Real
{
	ProductionGraphs::ProductionGraphs() :
		m_Entities(NULL),
		m_Systems()
	{
	}

	ProductionGraphs::ProductionGraphs(ProductionGraphs const& _src)
	{
		throw Exception("internal error: attempted to copy production graphs");
	}

	ProductionGraphs& ProductionGraphs::operator=(ProductionGraphs const& _src)
	{
		throw Exception("internal error: attempted to copy production graphs");
	}

	ProductionGraphs::~ProductionGraphs()
	{
		for (SystemMap::iterator it = m_Systems.begin(); it != m_Systems.end(); it++)
		{
			try
			{
				m_Entities->destroy(it->second);
			}
			catch (Exception &e)
			{
				std::cout << "error on production graphs destruction: " << e.what() << std::endl;
			}
			catch (...)
			{
				std::cout << "error on production graphs destruction" << std::endl;
			}
		}
	}

	const unsigned int ProductionGraphs::createSystem(std::string const& _name)
	{
		try
		{
			//request container creation
			Container *nirvana = m_Entities->createSystem(_name);
			m_Systems.insert(NamedSystem(nirvana->id(), nirvana));
			return nirvana->id();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void ProductionGraphs::destroySystem(unsigned int const& _id)
	{
		try
		{
			SystemMap::iterator it = m_Systems.find(_id);

			if (it == m_Systems.end())
			{
				throw Exception("internal error, attempted to destroy a system which does not exist");
			}

			Container *nirvana = it->second;

			//stops & shuts down all children
			nirvana->shutdown();
			//deletes all children
			m_Entities->destroy(nirvana);
			m_Systems.erase(it);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Container *const ProductionGraphs::getSystem(unsigned int const& _id)
	{
		SystemMap::const_iterator it = m_Systems.find(_id);
		if (it == m_Systems.end())
		{
			throw Exception("internal error, tried to access nonexistant system");
		}
		else if (!it->second)
		{
			throw Exception("internal error, null pointer stored in production graphs");
		}
		return it->second;
	}

	void ProductionGraphs::destroy(unsigned int const& _id, unsigned int const& _top)
	{
		try
		{
			Container *nirvana = getSystem(_top);
			AbstractContainer *container = nirvana->get(_id);
			container->shutdown();
			m_Entities->destroy(container);
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const unsigned int ProductionGraphs::createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		try
		{
			Container *nirvana = getSystem(_top);
			AbstractContainer *a = nirvana->get(_a);
			AbstractContainer *b = nirvana->get(_b);

			Container *seq = m_Entities->createSequence(_name);
			
			//move sequence into nirvana
			nirvana->add(seq, 0);

			//add containers to newly created sequence
			seq->add(b, 0);
			seq->add(a, 0);

			return seq->id();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const unsigned int ProductionGraphs::createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		try
		{
			Container *nirvana = getSystem(_top);
			AbstractContainer *a = nirvana->get(_a);
			AbstractContainer *b = nirvana->get(_b);

			Container *sync = m_Entities->createSynchronization(_name);

			//move sync into nirvana
			nirvana->add(sync, 0);

			//add to containers to newly created synchronization
			sync->add(a, 0);
			sync->add(b, 1);

			return sync->id();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}