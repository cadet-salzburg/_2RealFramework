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

namespace _2Real
{

	Identifier::Identifier(std::string const& _name, std::string const& _type, unsigned int const& _id) :
		m_Name(_name),
		m_Type(_type),
		m_Id(_id)
	{
	}

	bool Identifier::operator==(Identifier const& _rhs) const
	{
		return (m_Id == _rhs.m_Id);
	}

	bool Identifier::operator!=(Identifier const& _rhs) const
	{
		return !(m_Id == _rhs.m_Id);
	}

	bool Identifier::operator<(Identifier const& _rhs) const
	{
		return (m_Id < _rhs.m_Id);
	}

	bool Identifier::operator<=(Identifier const& _rhs) const
	{
		return (m_Id <= _rhs.m_Id);
	}

	bool Identifier::operator>(Identifier const& _rhs) const
	{
		return (m_Id > _rhs.m_Id);
	}

	bool Identifier::operator>=(Identifier const& _rhs) const
	{
		return (m_Id  >= _rhs.m_Id );
	}

}