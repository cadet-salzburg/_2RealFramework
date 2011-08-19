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

#include <map>

namespace _2Real
{

	/**
	*
	*/

	class IdentifierImpl;
	class IEntity;

	class Entities
	{

		friend class EngineImpl;

	public:

		/**
		*
		*/
		IdentifierImpl const *const createID(std::string const& _name, IEntity *const _entity);

		/**
		*	
		*/
		IEntity *const get(IdentifierImpl const *const _id);

	private:

		/**
		*
		*/
		Entities();

		/**
		*
		*/
		Entities(Entities const& _src) throw(...);

		/**
		*
		*/
		Entities& operator=(Entities const& _src) throw(...);

		/**
		*
		*/
		~Entities();

		/**
		*
		*/
		typedef std::pair< IdentifierImpl, IEntity * >			NamedEntity;

		/**
		*
		*/
		typedef std::map< IdentifierImpl, IEntity * >			EntityMap;

		/**
		*
		*/
		EntityMap												m_Entities;

		/**
		*
		*/
		unsigned int											m_iCreationCount;

	};

}