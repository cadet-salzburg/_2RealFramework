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
#include "_2RealEntities.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealServiceImpl.h"

#include <iostream>

namespace _2Real
{
	ProductionGraphs::ProductionGraphs() :
		m_Plugins(NULL),
		m_Factory(NULL),
		m_Entities(NULL),
		m_Containers()
	{
	}

	ProductionGraphs::ProductionGraphs(ProductionGraphs const& _src)
	{
		throw Exception::noCopy();
	}

	ProductionGraphs& ProductionGraphs::operator=(ProductionGraphs const& _src)
	{
		throw Exception::noCopy();
	}

	ProductionGraphs::~ProductionGraphs()
	{
		for (ContainerMap::iterator it = m_Containers.begin(); it != m_Containers.end(); it++)
		{
			try
			{
				//destroys all children as well
				m_Entities->destroy(it->first);
				it->second = NULL;
			}
			catch (...)
			{
				//TODO
				std::cout << "error on production graph destruction" << std::endl;
			}
		}

		m_Containers.clear();
	}

	void ProductionGraphs::destroy(unsigned int const& _id, unsigned int const& _top)
	{
		try
		{
			IEntity *entity = m_Entities->get(_id);
			IdentifierImpl::eType type = entity->type();
			
			if (type != IdentifierImpl::SEQUENCE && type != IdentifierImpl::SYNCHRONIZATION && type != IdentifierImpl::NIRVANA)
			{
				throw Exception::failure();
			}
			else if (type == IdentifierImpl::NIRVANA)
			{
				ContainerMap::iterator it = m_Containers.find(_id);
				if (it == m_Containers.end())
				{
					throw Exception::failure();
				}

				m_Containers.erase(it);
				m_Entities->destroy(_id);
			}
			else
			{
				//_top must be in container map
				ContainerMap::iterator it = m_Containers.find(_top);
				if (it == m_Containers.end())
				{
					throw Exception::failure();
				}
				Container *top = it->second;

				AbstractContainer *a = top->getChild(_id);
				m_Entities->destroy(it->first);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	const bool ProductionGraphs::isNirvana(unsigned int const& _id)
	{
		ContainerMap::iterator it = m_Containers.find(_id);
		if (it == m_Containers.end())
		{
			return false;
		}
		
		return true;
	}

	const unsigned int ProductionGraphs::createNirvana(std::string const& _name)
	{
		try
		{
			//request container creation
			const Entities::ID id = m_Entities->createContainer(_name, IdentifierImpl::NIRVANA);
			//entities returns entity*
			Container *nirvana = static_cast< Container * >(id.second);
			m_Containers.insert(NamedContainer(nirvana->id(), nirvana));
			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

	const unsigned int ProductionGraphs::createSequence(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		try
		{
			//_top must be in container map 
			ContainerMap::iterator it = m_Containers.find(_top);
			if (it == m_Containers.end())
			{
				throw Exception::failure();
			}
			Container *top = it->second;

			//find children
			//after this op, the children's former root will be stopped
			//their father will be NULL
			//all IO connections will have been broken
			AbstractContainer *a = top->getChild(_a);
			AbstractContainer *b = top->getChild(_b);

			//both containers should exist, i guess (:
			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			//request container creation -returns entity*
			const Entities::ID id = m_Entities->createContainer(_name, IdentifierImpl::SEQUENCE);
			Container *seq = static_cast< Container * >(id.second);
			
			//move sequence into nirvana
			top->add(seq, 0);

			//add containers to newly created sequence
			seq->add(b, 0);
			seq->add(a, 0);

			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

	const unsigned int ProductionGraphs::createSynchronization(std::string const& _name, unsigned int const& _a, unsigned int const& _b, unsigned int const& _top)
	{
		try
		{
			//_top must be in container map 
			ContainerMap::iterator it = m_Containers.find(_top);
			if (it == m_Containers.end())
			{
				throw Exception::failure();
			}

			Container *top = it->second;

			//find children
			//after this op, the children's former root will be stopped
			//their father will be NULL
			//all IO connections will have been broken
			AbstractContainer *a = top->getChild(_a);
			AbstractContainer *b = top->getChild(_b);

			////both containers should exist, i guess (:
			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			//request container creation -returns entity*
			const Entities::ID id = m_Entities->createContainer(_name, IdentifierImpl::SYNCHRONIZATION);
			Container *sync = static_cast< Container * >(id.second);

			//move sync into nirvana
			top->add(sync, 0);

			//add to containers to newly created synchronization
			sync->add(a, 0);
			sync->add(b, 1);

			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

}