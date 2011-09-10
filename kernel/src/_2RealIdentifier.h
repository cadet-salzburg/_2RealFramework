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

#include <string>

namespace _2Real
{

	/**
	*	identifiers refer to entities in the 2 real engine
	*
	*	these entities may be:
	*							plugins
	*							service factories
	*							services
	*							sequences / synchronizations
	*							a service's setup parameters
	*							a service's input / output slots
	*/

	class IdentifierImpl;

	class Identifier
	{

	public:

		/**
		*
		*/
		Identifier(Identifier const& _src);

		/**
		*
		*/
		Identifier& operator=(Identifier const& _src);

		/**
		*
		*/
		~Identifier();

		/**
		*	true if the identifiers refer to the same entity within the framework
		*/
		bool operator==(Identifier const& _rhs) const;

		/**
		*	true if the identifiers refer to different entities within the framework
		*/
		bool operator!=(Identifier const& _rhs) const;

		/**
		*	in case identifiers are to be used as key for a map. or whatever.
		*/
		bool operator<(Identifier const& _rhs) const;

		/**
		*	in case identifiers are to be used as key for a map. or whatever.
		*/
		bool operator<=(Identifier const& _rhs) const;

		/**
		*	in case identifiers are to be used as key for a map. or whatever.
		*/
		bool operator>(Identifier const& _rhs) const;

		/**
		*	in case identifiers are to be used as key for a map. or whatever.
		*/
		bool operator>=(Identifier const& _rhs) const;

		/**
		*	the entity's type, as string
		*/
		std::string const& type() const;

		/**
		*	the entity's name, as string
		*/
		std::string const& name() const;

		/**
		*	some additional info about the entity
		*/
		std::string const& info() const;

		/**
		*	the entity's unique id
		*/
		unsigned int const& id() const;

	private:

		friend class EntityTable;

		/**
		*	identifiers may be created only by the 2 real engine
		*/
		Identifier(IdentifierImpl *const _id);

		/**
		*	
		*/
		IdentifierImpl		*m_Impl;

	};

}