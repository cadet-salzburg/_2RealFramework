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
#include <string>

namespace _2Real
{

	/**
	*
	*/

	class Plugin;
	class IdentifierImpl;
	class ServiceFactory;
	class Entities;

	class PluginPool
	{

		friend class EngineImpl;
	
	public:

		/**
		*	
		*/
		IdentifierImpl const *const install(std::string const& _name, std::string const& _path, std::string const& _class) throw(...);

		/**
		*	
		*/
		void uninstall(IdentifierImpl const& _id) throw(...);

		/**
		*
		*/
		Plugin const *const get(IdentifierImpl const& _id) const;

	private:

		/**
		*	
		*/
		PluginPool();

		/**
		*	
		*/
		PluginPool(ServiceFactory *const _factory, Entities *const _entities);

		/**
		*	
		*/
		PluginPool(PluginPool const& _src) throw(...);

		/**
		*	
		*/
		PluginPool& operator=(PluginPool const& _src) throw(...);

		/**
		*	
		*/
		~PluginPool();

		/**
		*	yay, typedefs
		*/
		typedef std::pair< IdentifierImpl, Plugin * >		NamedPlugin;

		/**
		*	sfedepyt ,yay
		*/
		typedef std::map< IdentifierImpl, Plugin * >		PluginMap;

		/**
		*	installed plugins
		*/
		PluginMap											m_Plugins;

		/**
		*	framework's service factory
		*/
		ServiceFactory										*m_Factory;

		/**
		*	framework's entities
		*/
		Entities											*m_Entities;

	};

}