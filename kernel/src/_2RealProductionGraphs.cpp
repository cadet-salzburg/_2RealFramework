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

				it->second = NULL;
				m_Containers.erase(it);
				m_Entities->destroy(it->first);
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
			
			//add containers to newly created sequence
			seq->append(a);
			seq->append(b);

			//now, everything is in place -> autolink children
			//TODO: perform some checks
			//like, if the params even match in number and type etc
			AbstractContainer::IdentifierList output = a->outputParams();
			AbstractContainer::IdentifierList input = b->inputParams();
			AbstractContainer::IdentifierList::iterator itIn;
			AbstractContainer::IdentifierList::iterator itOut;
			for (itIn = input.begin(), itOut = output.begin(); itIn != input.end(), itOut != output.end(); itIn++, itOut++)
			{
				IEntity *in = m_Entities->get(*itIn);
				IEntity *out = m_Entities->get(*itOut);
				ServiceSlot *inParam = static_cast< ServiceSlot * >(in);
				ServiceSlot *outParam = static_cast< ServiceSlot * >(out);
				inParam->linkWith(outParam);
				outParam->linkWith(inParam);
				ServiceImpl *serviceIn = inParam->service();
				ServiceImpl *serviceOut = outParam->service();
				serviceOut->addListener(serviceIn);
				serviceIn->listenTo(serviceOut);
			}

			//finally, move sequence into nirvana
			top->append(seq);

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

			//add to containers to newly created synchronization
			sync->append(a);
			sync->append(b);

			//in a sync, no internal linking of slots takes place

			//finally, move sync into nirvana
			top->append(sync);

			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

}