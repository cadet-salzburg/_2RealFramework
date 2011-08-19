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

//included because of eType
#include "_2RealIdentifierImpl.h"

#include <map>

namespace _2Real
{

	/**
	*
	*/

	class Identities
	{

		friend class EngineImpl;

	public:

		/**
		*
		*/
		IdentifierImpl const *const createID(std::string const& _name, IdentifierImpl::eType const& _type);

	private:

		/**
		*
		*/
		Identities();

		/**
		*
		*/
		Identities(Identities const& _src) throw(...);

		/**
		*
		*/
		Identities& operator=(Identities const& _src) throw(...);

		/**
		*
		*/
		~Identities();

		/**
		*
		*/
		typedef std::pair< unsigned int, IdentifierImpl * >		NamedID;

		/**
		*
		*/
		typedef std::map< unsigned int, IdentifierImpl * >		IDMap;

		/**
		*
		*/
		IDMap													m_IDs;

		/**
		*
		*/
		unsigned int											m_iCreationCount;

	};

}