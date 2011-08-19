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
	*	identifiers are used to refer to entities within the framework
	*
	*	these entities may be:
	*							plugins
	*							a service's factory
	*							service objects
	*							production graphs
	*							mutexes
	*							a service's setup parameters
	*							a service's input / output variables
	*
	*	note that 2 identifiers may compare as different, although their names and types are the same
	*/

	class IdentifierImpl;

	class Identifier
	{

	public:

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
		*	in case identifiers are tio be used as key for a map. or whatever.
		*/
		bool operator>=(Identifier const& _rhs) const;

		/**
		*	the entity's type, as string
		*/
		std::string const& type() const;

		/**
		*	the entity's name, as string
		*	typically, the name is given by the application programmer;
		*	only in the case of service factories, it is chosen by the framework
		*/
		std::string const& name() const;

		/**
		*	some additional info about the entity
		*/
		std::string const& info() const;

	private:

		friend class EngineImpl;

		/**
		*	identifiers may be created only by the 2 real engine
		*/
		Identifier();

		/**
		*	identifiers may be created only by the 2 real engine
		*/
		Identifier(IdentifierImpl const *const _impl);

		/**
		*	dun dun dun.
		*/
		const IdentifierImpl			*m_Impl;

	};

}