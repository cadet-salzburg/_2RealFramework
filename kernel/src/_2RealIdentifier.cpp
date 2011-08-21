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

#include "_2RealIdentifier.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealException.h"

namespace _2Real
{

	Identifier::Identifier(IdentifierImpl *const _id) : m_Impl(NULL)
	{
		if (_id != NULL)
		{
			m_Impl = _id;
			m_Impl->retain();
		}
		else
		{
			throw Exception::failure();
		}
	}

	Identifier::Identifier(Identifier const& _src) : m_Impl(NULL)
	{
		m_Impl = _src.m_Impl;
		m_Impl->retain();
	}

	Identifier& Identifier::operator=(Identifier const& _src)
	{
		if (this == &_src)
		{
			return *this;
		}

		m_Impl->release();

		m_Impl = _src.m_Impl;
		m_Impl->retain();

		return *this;
	}

	Identifier::~Identifier()
	{
		m_Impl->release();
	}

	bool Identifier::operator==(Identifier const& _rhs) const
	{
		return (m_Impl == _rhs.m_Impl);
	}

	bool Identifier::operator!=(Identifier const& _rhs) const
	{
		return !(m_Impl == _rhs.m_Impl);
	}

	bool Identifier::operator<(Identifier const& _rhs) const
	{
		return (*m_Impl < *_rhs.m_Impl);
	}

	bool Identifier::operator<=(Identifier const& _rhs) const
	{
		return (*m_Impl <= *_rhs.m_Impl);
	}

	bool Identifier::operator>(Identifier const& _rhs) const
	{
		return (*m_Impl > *_rhs.m_Impl);
	}

	bool Identifier::operator>=(Identifier const& _rhs) const
	{
		return (*m_Impl  >= *_rhs.m_Impl );
	}

	std::string const& Identifier::type() const
	{
		return m_Impl->strType();
	}

	std::string const& Identifier::name() const
	{
		return m_Impl->name();
	}

	std::string const& Identifier::info() const
	{	
		return m_Impl->info();
	}

	unsigned int const& Identifier::id() const
	{
		return m_Impl->id();
	}

}