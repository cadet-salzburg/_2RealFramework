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
#include <list>
#include <string>

#include "Poco/Path.h"

namespace _2Real
{

	/**
	*
	*/

	class Plugin;
	class PluginMetadata;
	class ServiceFactory;
	class ProductionGraphs;
	class Identifier;
	class Entities;

	class PluginPool
	{
	
	public:

		/**
		*	
		*/
		PluginPool();

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
		*	
		*/
		const Identifier install(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class, std::list< Identifier > &_ids) throw(...);

		/**
		*	
		*/
		//void uninstall(unsigned int const& _id) throw(...);

		/**
		*
		*/
		//Plugin const *const get(unsigned int const& _id) const;

		PluginMetadata const& pluginInfo(unsigned int const& _id) const throw(...);

	private:

		friend class EngineImpl;

		/**
		*	yay, typedefs
		*/
		typedef std::pair< unsigned int, Plugin * >			NamedPlugin;

		/**
		*	sfedepyt ,yay
		*/
		typedef std::map< unsigned int, Plugin * >			PluginMap;

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

		/**
		*	production graphs
		*/
		ProductionGraphs									*m_Graphs;

	};

}