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

#include "_2RealEntities.h"
#include "_2RealIEntity.h"
#include "_2RealIdentifier.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

#include "_2RealPlugin.h"
#include "_2RealServiceImpl.h"
#include "_2RealFactoryReference.h"
#include "_2RealContainer.h"
#include "_2RealServiceSlot.h"
#include "_2RealServiceValue.h"

#include <sstream>

namespace _2Real
{

	Entities::Entities() : m_iCreationCount(0), m_Factory(NULL), m_Plugins(NULL)
	{
	}

	Entities::Entities(Entities const& _src)
	{
		throw Exception::noCopy();
	}

	Entities& Entities::operator=(Entities const& _src)
	{
		throw Exception::noCopy();
	}

	Entities::~Entities()
	{
		for (EntityMap::iterator it = m_Entities.begin(); it != m_Entities.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
	}

	void Entities::destroy(Container *_obj)
	{
		try
		{
			if (_obj->type() != IdentifierImpl::NIRVANA)
			{
				_obj->resetIO();
			}
			std::list< unsigned int > ids;
			std::list< unsigned int >::iterator it;
			ids = _obj->children();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (...)
		{
			throw;
		}
	}

	void Entities::destroy(ServiceImpl *_obj)
	{
		try
		{
			_obj->resetIO();
			
			std::list< unsigned int > ids;
			std::list< unsigned int >::iterator it;
			
			ids = _obj->setupParamIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			ids = _obj->inputParamIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			ids = _obj->outputParamIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (...)
		{
			throw;
		}
	}

	void Entities::destroy(ServiceSlot *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (...)
		{
			throw;
		}
	}

	void Entities::destroy(ServiceValue *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (...)
		{
			throw;
		}
	}

	void Entities::destroy(Plugin *_obj)
	{
		try
		{
			std::list< unsigned int > ids;
			std::list< unsigned int >::iterator it;
			ids = _obj->serviceIDs();
			for (it = ids.begin(); it != ids.end(); it++)
			{
				destroy(*it);
			}

			delete _obj;
		}
		catch (...)
		{
		}
	}

	void Entities::destroy(FactoryReference *_obj)
	{
		try
		{
			delete _obj;
		}
		catch (...)
		{
		}
	}

	void Entities::destroy(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			IEntity *e = it->second;
			IdentifierImpl::eType type = e->type();

			if (type == IdentifierImpl::SERVICE)
			{
				ServiceImpl *service = static_cast< ServiceImpl * >(e);
				destroy(service);
			}
			else if (type == IdentifierImpl::NIRVANA || e->type() == IdentifierImpl::SEQUENCE || type == IdentifierImpl::SYNCHRONIZATION)
			{
				Container *container = static_cast< Container * >(e);
				destroy(container);
			}
			else if (type == IdentifierImpl::INPUT || type == IdentifierImpl::OUTPUT)
			{
				ServiceSlot *slot = static_cast< ServiceSlot * >(e);
				destroy(slot);
			}
			else if (type == IdentifierImpl::SETUP)
			{
				ServiceValue *value = static_cast< ServiceValue * >(e);
				destroy(value);
			}
			else if (type == IdentifierImpl::PLUGIN)
			{
				Plugin *plugin = static_cast< Plugin * >(e);
				destroy(plugin);
			}
			else if (type == IdentifierImpl::FACTORY)
			{
				FactoryReference *ref = static_cast< FactoryReference * >(e);
				destroy(ref);
			}

			it->second = NULL;
			m_Entities.erase(it);
		}
		else
		{
			throw Exception::failure();
		}
	}

	IEntity *const Entities::get(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			return it->second;
		}

		throw Exception::failure();
	}

	const Identifier Entities::getIdentifier(unsigned int const& _id) const
	{
		EntityMap::const_iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			return Identifier(it->second->m_ID);
		}

		throw Exception::failure();
	}

	const Entities::ID Entities::createPlugin(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{

		++m_iCreationCount;

		//plugin info
		std::stringstream info;
		info << "i am a plugin" << std::endl;
		info << "creation id: " << m_iCreationCount << std::endl;
		info << "chosen name: " << _name << std::endl;
		info << "directory:   " << _dir << std::endl;
		info << "filename:    " << _file << std::endl;
		info << "classname:   " << _class << std::endl;

		IdentifierImpl *id = new IdentifierImpl(_name, "plugin", info.str(), IdentifierImpl::PLUGIN, m_iCreationCount);
		Plugin *plugin = new Plugin(_dir, _file, _class, m_Factory, id);
		m_Entities.insert(NamedEntity(id->id(), plugin));
		return Entities::ID(id->id(), plugin);

	}

	const Entities::ID Entities::createService(std::string const& _name, IService *const _service)
	{
		try
		{
			IdentifierImpl *id = new IdentifierImpl(_name, "service", "", IdentifierImpl::SERVICE, ++m_iCreationCount);
			ServiceImpl *service = new ServiceImpl(_service, id);
			m_Entities.insert(NamedEntity(id->id(), service));
			return Entities::ID(id->id(), service);
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const Entities::ID Entities::createContainer(std::string const& _name, IdentifierImpl::eType const& _type)
	{
		try
		{
			IdentifierImpl *id;
			Container *container;
			switch(_type)
			{

			case IdentifierImpl::SEQUENCE:

				id = new IdentifierImpl(_name, "sequence", "", IdentifierImpl::SEQUENCE, ++m_iCreationCount);
				break;

			case IdentifierImpl::SYNCHRONIZATION:
				
				id = new IdentifierImpl(_name, "synchronization", "", IdentifierImpl::SYNCHRONIZATION, ++m_iCreationCount);
				break;

			case IdentifierImpl::NIRVANA:
				id = new IdentifierImpl(_name, "nirvana", "", IdentifierImpl::NIRVANA, ++m_iCreationCount);
				break;
		
			default:
				throw Exception::failure();
				break;

			}

			container = new Container(id);
			m_Entities.insert(NamedEntity(id->id(), container));
			return Entities::ID(id->id(), container);
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const Entities::ID Entities::createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, ServiceMetadata const& _metadata)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "factory", _plugin->name(), IdentifierImpl::FACTORY, ++m_iCreationCount);
		FactoryReference *ref = new FactoryReference(_name, _plugin, _creator, _metadata, id);
		m_Entities.insert(NamedEntity(id->id(), ref));
		return Entities::ID(id->id(), ref);
	}

	const Entities::ID Entities::createServiceValue(std::string const& _name, ServiceImpl *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "setup value", "", IdentifierImpl::SETUP, ++m_iCreationCount);
		ServiceValue *val = new ServiceValue(id, _service);
		m_Entities.insert(NamedEntity(id->id(), val));
		return Entities::ID(id->id(), val);
	}

	const Entities::ID Entities::createInputSlot(std::string const& _name, ServiceImpl *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "input slot", "", IdentifierImpl::INPUT, ++m_iCreationCount);
		ServiceSlot *slot = new ServiceSlot(id, _service);
		m_Entities.insert(NamedEntity(id->id(), slot));
		return Entities::ID(id->id(), slot);
	}

	const Entities::ID Entities::createOutputSlot(std::string const& _name, ServiceImpl *const _service)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "output slot", "", IdentifierImpl::OUTPUT, ++m_iCreationCount);
		ServiceSlot *slot = new ServiceSlot(id, _service);
		m_Entities.insert(NamedEntity(id->id(), slot));
		return Entities::ID(id->id(), slot);
	}
}