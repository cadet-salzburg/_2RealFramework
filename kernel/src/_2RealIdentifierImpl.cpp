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

#include "_2RealIdentifierImpl.h"

namespace _2Real
{

	IdentifierImpl::IdentifierImpl() : m_Type(IEntity::INVALID), m_ID(0)
	{
	}

	IdentifierImpl::IdentifierImpl(std::string const& _name, std::string const& _strType, IEntity::eType const& _type, unsigned int const& _id) :
		m_Name(_name), m_Typename(_strType), m_Type(_type), m_ID(_id)
	{
	}

	IdentifierImpl::IdentifierImpl(IdentifierImpl const& _src) :
		m_Name(_src.m_Name), m_Type(_src.m_Type), m_ID(_src.m_ID), m_Typename(_src.m_Typename)
	{
	}

	IdentifierImpl& IdentifierImpl::operator=(IdentifierImpl const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Name = _src.m_Name;
		m_Typename = _src.m_Typename;
		m_Type = _src.m_Type;
		m_ID = _src.m_ID;

		return *this;
	}

	IdentifierImpl::~IdentifierImpl()
	{
	}

	bool IdentifierImpl::operator==(IdentifierImpl const& _rhs) const
	{
		return (m_ID == _rhs.m_ID);
	}

	bool IdentifierImpl::operator!=(IdentifierImpl const& _rhs) const
	{
		return !(m_ID == _rhs.m_ID);
	}

	bool IdentifierImpl::operator<(IdentifierImpl const& _rhs) const
	{
		return (m_ID < _rhs.m_ID);
	}

	bool IdentifierImpl::operator<=(IdentifierImpl const& _rhs) const
	{
		return (m_ID <= _rhs.m_ID);
	}

	bool IdentifierImpl::operator>(IdentifierImpl const& _rhs) const
	{
		return (m_ID > _rhs.m_ID);
	}

	bool IdentifierImpl::operator>=(IdentifierImpl const& _rhs) const
	{
		return (m_ID >= _rhs.m_ID);
	}

	std::string const& IdentifierImpl::strType() const
	{
		return m_Typename;
	}

	std::string const& IdentifierImpl::name() const
	{
		return m_Name;
	}

	IEntity::eType const& IdentifierImpl::type() const
	{
		return m_Type;
	}

	unsigned int const& IdentifierImpl::id() const
	{
		return m_ID;
	}

}