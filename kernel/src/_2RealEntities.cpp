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
#include "_2RealServiceParam.h"

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

	void Entities::destroy(unsigned int const& _id)
	{
		EntityMap::iterator it = m_Entities.find(_id);

		if (it != m_Entities.end())
		{
			delete it->second;
			it->second = NULL;
		}

		throw Exception::failure();
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

	const Entities::ID Entities::createPlugin(std::string const& _name, std::string const& _path, std::string const& _class)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "plugin", _path, IdentifierImpl::PLUGIN, ++m_iCreationCount);
		Plugin *plugin = new Plugin(_path, _class, m_Factory, id);
		return Entities::ID(Identifier(id), plugin);
	}

	const Entities::ID Entities::createService(std::string const& _name, IService *const _service)
	{
		try
		{
			IdentifierImpl *id = new IdentifierImpl(_name, "service", "", IdentifierImpl::SERVICE, ++m_iCreationCount);
			ServiceImpl *service = new ServiceImpl(_service, id);
			return Entities::ID(Identifier(id), service);
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
				container = new Container(id);

				break;

			case IdentifierImpl::SYNCHRONIZATION:
				
				id = new IdentifierImpl(_name, "synchronization", "", IdentifierImpl::SYNCHRONIZATION, ++m_iCreationCount);
				container = new Container(id);

				break;

			case IdentifierImpl::NIRVANA:
				id = new IdentifierImpl(_name, "nirvana", "", IdentifierImpl::NIRVANA, ++m_iCreationCount);
				container = new Container(id);
				break;
		
			default:
				throw Exception::failure();
				break;

			}

			return Entities::ID(Identifier(id), container);
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	const Entities::ID Entities::createFactoryRef(std::string const& _name, Plugin const *const _plugin, ServiceCreator _creator, Metadata const *const _metadata)
	{
		IdentifierImpl *id = new IdentifierImpl(_name, "factory", _plugin->name(), IdentifierImpl::FACTORY, ++m_iCreationCount);
		FactoryReference *ref = new FactoryReference(_name, _plugin, _creator, _metadata, id);
		return Entities::ID(Identifier(id), ref);
	}

	const Entities::ID Entities::createServiceParam(std::string const& _name, ServiceImpl const *const _service, IdentifierImpl::eType const& _type)
	{

		IdentifierImpl *id;
		switch(_type)
		{

		case IdentifierImpl::SETUP:
			id = new IdentifierImpl(_name, "setup", "", IdentifierImpl::SETUP, ++m_iCreationCount);
			break;

		case IdentifierImpl::INPUT:
			id = new IdentifierImpl(_name, "input", "", IdentifierImpl::INPUT, ++m_iCreationCount);
			break;

		case IdentifierImpl::OUTPUT:
			id = new IdentifierImpl(_name, "output", "", IdentifierImpl::OUTPUT, ++m_iCreationCount);
			break;
		
		default:
			throw Exception::failure();
			break;
		}

		ServiceParam *param = new ServiceParam(id, _service);
		return Entities::ID(Identifier(id), param);
	}

}