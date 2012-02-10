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
#include "_2RealEntity.h"

namespace _2Real
{

	Identifier::Identifier() :
		m_Name(""),
		m_Type(""),
		m_Id(0)
	{
	}

	Identifier::Identifier(std::string const& name, std::string const& type, unsigned int const& id) :
		m_Name(name),
		m_Type(type),
		m_Id(id)
	{
	}

	Identifier::Identifier(Identifier const& src) :
		m_Name(src.m_Name),
		m_Type(src.m_Type),
		m_Id(src.m_Id)
	{
	}

	Identifier& Identifier::operator=(Identifier const& src)
	{
		m_Name = src.m_Name;
		m_Type = src.m_Type;
		m_Id = src.m_Id;

		return *this;
	}

	const bool Identifier::isValid() const
	{
		return *this == Entity::NoEntity();
	}

	unsigned int Identifier::id() const
	{
		return m_Id;
	}

	std::string const& Identifier::name() const
	{
		return m_Name;
	}

	std::string const& Identifier::type() const
	{
		return m_Type;
	}

	const bool Identifier::operator==(Identifier const& rhs) const
	{
		return (m_Id == rhs.m_Id);
	}

	const bool Identifier::operator!=(Identifier const& rhs) const
	{
		return !(m_Id == rhs.m_Id);
	}

	const bool Identifier::operator<(Identifier const& rhs) const
	{
		return (m_Id < rhs.m_Id);
	}

	const bool Identifier::operator<=(Identifier const& rhs) const
	{
		return (m_Id <= rhs.m_Id);
	}

	const bool Identifier::operator>(Identifier const& rhs) const
	{
		return (m_Id > rhs.m_Id);
	}

	const bool Identifier::operator>=(Identifier const& rhs) const
	{
		return (m_Id  >= rhs.m_Id );
	}

	const bool Identifier::isPlugin() const
	{
		return (m_Type == "plugin");
	}

	const bool Identifier::isService() const
	{
		return (m_Type == "service");
	}

	const bool Identifier::isSetupAble() const
	{
		return (m_Type == "plugin" || m_Type == "service");
	}

	const bool Identifier::isRunAble() const
	{
		return (m_Type == "service" || m_Type == "sequence" || m_Type == "synchronization");
	}

	const bool Identifier::isContainer() const
	{
		return (m_Type == "system" || m_Type == "sequence" || m_Type == "synchronization");
	}

	std::ostream& operator<<(std::ostream& out, Identifier const& id)
	{
		out << id.name() << " " << id.type() << " " << id.id();
		return out;
	}

}