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

#include "_2RealId.h"
#include "_2RealException.h"

namespace _2Real
{

	Id::Id(std::string const& _name, std::string const& _strType, std::string const& _info, Entity::eType const& _type, unsigned int const& _id) :
		m_Name(_name), m_Typename(_strType), m_Info(_info), m_Type(_type), m_ID(_id), m_iRefCount(1)
	{
	}

	Id::Id(Id const& _src) :
		m_Name(_src.m_Name), m_Type(_src.m_Type), m_ID(_src.m_ID), m_Typename(_src.m_Typename), m_Info(_src.m_Info)
	{
		throw Exception("attempted to copy entity");
	}

	Id& Id::operator=(Id const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	void Id::retain()
	{
		m_iRefCount++;
	}

	void Id::release()
	{
		m_iRefCount--;
		
		if (m_iRefCount = NULL)
		{
			delete this;
		}
	}

	Id::~Id()
	{
	}

	bool Id::operator==(Id const& _rhs) const
	{
		return (m_ID == _rhs.m_ID);
	}

	bool Id::operator!=(Id const& _rhs) const
	{
		return !(m_ID == _rhs.m_ID);
	}

	bool Id::operator<(Id const& _rhs) const
	{
		return (m_ID < _rhs.m_ID);
	}

	bool Id::operator<=(Id const& _rhs) const
	{
		return (m_ID <= _rhs.m_ID);
	}

	bool Id::operator>(Id const& _rhs) const
	{
		return (m_ID > _rhs.m_ID);
	}

	bool Id::operator>=(Id const& _rhs) const
	{
		return (m_ID >= _rhs.m_ID);
	}

	std::string const& Id::strType() const
	{
		return m_Typename;
	}

	std::string const& Id::name() const
	{
		return m_Name;
	}

	std::string const& Id::info() const
	{
		return m_Info;
	}

	Entity::eType const& Id::type() const
	{
		return m_Type;
	}

	unsigned int const& Id::id() const
	{
		return m_ID;
	}

	void Id::setInfo(std::string const& _info)
	{
		m_Info.clear();
		m_Info = _info;
	}

}