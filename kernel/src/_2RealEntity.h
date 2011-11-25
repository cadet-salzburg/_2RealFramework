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

#include "Poco/Mutex.h"

namespace _2Real
{

	class Entity
	{

	public:

		Entity(Identifier const& id);
		virtual ~Entity();

		static const Identifier createIdentifier(std::string const& name, std::string const& type);

		Identifier const& identifier() const
		{
			return m_Id;
		}

		unsigned int const& id() const
		{
			return m_Id.id();
		}

		std::string const& name() const
		{
			return m_Id.name();
		}

		std::string const& type() const
		{
			return m_Id.type();
		}

		static const Identifier		NoEntity();

	private:

		const Identifier			m_Id;

		static unsigned int			m_CreationCount;
		static Poco::FastMutex		m_Mutex;

	};

}