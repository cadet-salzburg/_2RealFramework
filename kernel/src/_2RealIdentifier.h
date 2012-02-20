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
	*	identifiers refer to entities in the 2 real EngineImpl;
	*	specifically, to entities created at the application programmer's request
	*	- plugins
	*	- concrete instances of the services exported by a plugin
	*	- systems
	*	- sequence graphs
	*	- synchronization graphs
	*/

	class Identifier
	{

	public:

		Identifier();
		Identifier(Identifier const& src);
		Identifier& operator=(Identifier const& src);

		const bool isValid() const;

		/**
		*	returns the entity's name
		*
		*	@return:		name as chosen by the application programmer
		*/
		std::string const& name() const;

		/**
		*	returns the entity's type, as string
		*
		*	@return:		type: plugin, system, sequence, synchronization,
		*					service (= a concrete instance of a plugin-exported service)
		*/
		std::string const& type() const;

		/**
		*	returns the entity's id
		*
		*	@return:		the entity's unique id
		*/
		unsigned int id() const;

		/**
		*	true if the identifiers refer to the same entity within the framework
		*/
		const bool operator==(Identifier const& rhs) const;

		/**
		*	true if the identifiers refer to different entities within the framework
		*/
		const bool operator!=(Identifier const& rhs) const;

		/*
		*	identifiers can be sorted in a strict monotonic order
		*/
		const bool operator<(Identifier const& rhs) const;
		const bool operator<=(Identifier const& rhs) const;
		const bool operator>(Identifier const& rhs) const;
		const bool operator>=(Identifier const& rhs) const;

		friend std::ostream& operator<<(std::ostream& out, Identifier const& id);

		const bool isPlugin() const;
		const bool isService() const;
		const bool isSetupAble() const;
		const bool isRunAble() const;
		const bool isContainer() const;

	private:

		friend class Entity;

		Identifier(std::string const& name, std::string const& type, unsigned int const& id);

		/**
		*	name chosen by the application programmer
		*/
		std::string			m_Name;

		/**
		*	type as string (plugin, service, sequence, synchronization, system)
		*/
		std::string			m_Type;

		/**
		*	unique id
		*/
		unsigned int		m_Id;

	};

}