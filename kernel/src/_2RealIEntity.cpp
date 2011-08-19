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

	IEntity::IEntity(eType const& _type) : m_Type(_type), m_ID(NULL)
	{
	}

	IEntity::IEntity(IEntity const& _src) throw(...) : m_Type(IEntity::INVALID)
	{
		throw Exception::failure();
	}

	IEntity& IEntity::operator=(IEntity const& _src) throw(...)
	{
		throw Exception::failure();
	}

	IEntity::~IEntity()
	{
		m_ID = NULL;
	}

	IEntity::eType const& IEntity::type() const
	{
		return m_Type;
	}

	IdentifierImpl const& IEntity::id() const
	{
		if (m_ID != NULL)
		{
			return *m_ID;
		}

		//how strange
		throw Exception::failure();
	}

}