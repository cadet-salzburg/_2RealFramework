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

#include "_2RealIEntity.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

namespace _2Real
{

	IEntity::IEntity(IdentifierImpl *const _id) : m_ID(_id)
	{
		if (_id == NULL)
		{
			throw Exception::failure();
		}
	}

	IEntity::IEntity(IEntity const& _src)
	{
		throw Exception::noCopy();
	}

	IEntity& IEntity::operator=(IEntity const& _src)
	{
		throw Exception::noCopy();
	}

	IEntity::~IEntity()
	{
		if (m_ID != NULL)
		{
			m_ID->release();
		}
	}

	IdentifierImpl::eType const& IEntity::type() const
	{
		if (m_ID != NULL)
		{
			return m_ID->type();
		}

		throw Exception::failure();
	}

	unsigned int const& IEntity::id() const
	{
		if (m_ID != NULL)
		{
			return m_ID->id();
		}

		throw Exception::failure();
	}

	std::string const& IEntity::name() const
	{
		if (m_ID != NULL)
		{
			return m_ID->name();
		}

		throw Exception::failure();
	}

	void IEntity::setInfo(std::string const& _info)
	{
		if (m_ID != NULL)
		{
			m_ID->setInfo(_info);
		}

		throw Exception::failure();
	}

}