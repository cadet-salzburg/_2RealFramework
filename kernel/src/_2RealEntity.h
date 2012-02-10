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

#pragma once

#include "_2RealIdentifier.h"

#include <string>

namespace _2Real
{

	class Entity
	{

	public:

		Entity(Identifier const& id);
		virtual ~Entity();

		static const Identifier	createIdentifier(std::string const& name, std::string const& type);
		static const Identifier	NoEntity();

		Identifier const&		identifier() const;
		unsigned int			id() const;
		std::string const&		name() const;
		std::string const&		type() const;
		bool					isPlugin() const;
		bool					isService() const;
		bool					isSetupAble() const;
		bool					isRunnable() const;
		bool					isGraph() const;

	private:

		const Identifier		m_Id;

	};

	inline Identifier const& Entity::identifier() const
	{
		return m_Id;
	}

	inline unsigned int Entity::id() const
	{
		return m_Id.id();
	}

	inline std::string const& Entity::name() const
	{
		return m_Id.name();
	}

	inline std::string const& Entity::type() const
	{
		return m_Id.type();
	}

	inline bool Entity::isPlugin() const
	{
		return (m_Id.type() == "plugin");
	}

	inline bool Entity::isService() const
	{
		return (m_Id.type() == "service");
	}

	inline bool Entity::isSetupAble() const
	{
		return (m_Id.type() == "plugin" || m_Id.type() == "service");
	}

	inline bool Entity::isRunnable() const
	{
		return (m_Id.type() == "service" || m_Id.type() == "sequence" || m_Id.type() == "synchronization");
	}

	inline bool Entity::isGraph() const
	{
		return (m_Id.type() == "sequence"|| m_Id.type() == "synchronization" || m_Id.type() == "system");
	}

}