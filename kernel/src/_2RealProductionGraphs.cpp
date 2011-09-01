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
#include "_2RealIdentifier.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"
#include "_2RealServiceImpl.h"

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

	void ProductionGraphs::destroy(Identifier const& _id, Identifier const& _top)
	{
		try
		{
			unsigned int id = _id.id();
			IEntity *entity = m_Entities->get(id);
			IdentifierImpl::eType type = entity->type();
			
			if (type != IdentifierImpl::SEQUENCE && type != IdentifierImpl::SYNCHRONIZATION && type != IdentifierImpl::NIRVANA)
			{
				throw Exception::failure();
			}
			else if (type == IdentifierImpl::NIRVANA)
			{
				ContainerMap::iterator it = m_Containers.find(id);
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
				ContainerMap::iterator it = m_Containers.find(_top.id());
				if (it == m_Containers.end())
				{
					throw Exception::failure();
				}
				Container *top = it->second;

				//this moves child to top & stops former superior
				AbstractContainer *a = top->getChild(_id.id(), top);
				//cut child out of it's tree structure completely
				top->remove(a->id());
				//destroy child
				m_Entities->destroy(it->first);
			}
		}
		catch (...)
		{
		}
	}

	const bool ProductionGraphs::isNirvana(Identifier const& _id)
	{
		ContainerMap::iterator it = m_Containers.find(_id.id());
		if (it == m_Containers.end())
		{
			return false;
		}
		
		return true;
	}

	const Identifier ProductionGraphs::createNirvana(std::string const& _name)
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

	const Identifier ProductionGraphs::createSequence(std::string const& _name, Identifier const& _a, Identifier const& _b, Identifier const& _top)
	{
		try
		{
			//_top must be in container map 
			ContainerMap::iterator it = m_Containers.find(_top.id());
			if (it == m_Containers.end())
			{
				throw Exception::failure();
			}
			Container *top = it->second;

			//removes children from their respective superiors
			//this operation also stops the superior (if != _top)
			//TODO: also remove all listeners & unlink stuff
			AbstractContainer *a = top->getChild(_a.id(), top);
			AbstractContainer *b = top->getChild(_b.id(), top);

			//both containers should exist, i guess (:
			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			//request container creation
			const Entities::ID id = m_Entities->createContainer(_name, IdentifierImpl::SEQUENCE);
			//entities returns entity*
			Container *seq = static_cast< Container * >(id.second);
			//append also performs insertion
			seq->append(a);
			seq->append(b);

			//remove from top
			top->remove(a->id());
			top->remove(b->id());

			if (a->father() != seq || b->father() != seq)
			{
				throw Exception::failure();
			}

			//move sequence into top...
			top->append(seq);

			//now, everything is in place -> autolink children
			AbstractContainer::IdentifierList output = a->outputParams();
			AbstractContainer::IdentifierList input = b->inputParams();
			std::cout << output.size() << " " << input.size() << std::endl;
			//TODO: perform some checks
			AbstractContainer::IdentifierList::iterator itIn;
			AbstractContainer::IdentifierList::iterator itOut;
			for (itIn = input.begin(), itOut = output.begin(); itIn != input.end(); itIn++, itOut++)
			{
				IEntity *in = m_Entities->get(itIn->id());
				IEntity *out = m_Entities->get(itOut->id());
				ServiceSlot *inParam = static_cast< ServiceSlot * >(in);
				ServiceSlot *outParam = static_cast< ServiceSlot * >(out);
				inParam->listenTo(outParam->id());
				ServiceImpl *serviceIn = inParam->service();
				ServiceImpl *serviceOut = outParam->service();
				serviceOut->addListener(serviceIn);
			}

			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

	const Identifier ProductionGraphs::createSynchronization(std::string const& _name, Identifier const& _a, Identifier const& _b, Identifier const& _top)
	{
		try
		{
			//_top must be in container map 
			ContainerMap::iterator it = m_Containers.find(_top.id());
			if (it == m_Containers.end())
			{
				throw Exception::failure();
			}
			Container *top = it->second;
			//std::cout << top->childCount() << std::endl;

			//removes children from their respective superiors
			//this operation also stops the superior (if != _top)
			//TODO: also remove all listeners & unlink stuff
			AbstractContainer *a = top->getChild(_a.id(), top);
			AbstractContainer *b = top->getChild(_b.id(), top);

			//both containers should exist, i guess (:
			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			//request container creation
			const Entities::ID id = m_Entities->createContainer(_name, IdentifierImpl::SYNCHRONIZATION);
			//entities returns entity*
			Container *sync = static_cast< Container * >(id.second);
			//append also performs insertion
			sync->append(a);
			sync->append(b);

			top->remove(a->id());
			top->remove(b->id());

			if (a->father() != sync || b->father() != sync)
			{
				throw Exception::failure();
			}

			top->append(sync);

			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

}