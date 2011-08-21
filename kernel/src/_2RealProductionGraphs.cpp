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

namespace _2Real
{
	ProductionGraphs::ProductionGraphs() :
		m_Plugins(NULL), m_Factory(NULL), m_Entities(NULL), m_Containers()
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
				ContainerMap::iterator it = m_Containers.find(_top.id());
				if (it == m_Containers.end())
				{
					throw Exception::failure();
				}

				Container *top = it->second;
				AbstractContainer *a = top->getChild(_id.id(), top);
				top->remove(_id.id());
				m_Entities->destroy(it->first);
			}	
		}
		catch (...)
		{
		}
	}

	const Identifier ProductionGraphs::createNirvana(std::string const& _name)
	{
		try
		{
			const Entities::ID id = m_Entities->createContainer(_name, NULL, IdentifierImpl::NIRVANA);
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
			//also stops them
			AbstractContainer *a = top->getChild(_a.id(), top);
			AbstractContainer *b = top->getChild(_b.id(), top);

			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			const Entities::ID id = m_Entities->createContainer(_name, top, IdentifierImpl::SEQUENCE);
			
			Container *seq = static_cast< Container * >(id.second);		
			seq->append(a);
			seq->append(b);
			
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

			//removes children from their respective superiors
			//also stops them
			AbstractContainer *a = top->getChild(_a.id(), top);
			AbstractContainer *b = top->getChild(_b.id(), top);

			if (a == NULL || b == NULL)
			{
				throw Exception::failure();
			}

			const Entities::ID id = m_Entities->createContainer(_name, top, IdentifierImpl::SYNCHRONIZATION);
			
			Container *sync = static_cast< Container * >(id.second);
			sync->append(a);
			sync->append(b);
			
			return id.first;
		}
		catch (...)
		{
			throw;
		}
	}

}