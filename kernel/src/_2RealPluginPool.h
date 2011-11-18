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

#include "_2RealEngine.h"
#include "_2RealException.h"

#include <map>
#include <string>

#include "Poco/Path.h"

namespace _2Real
{

	class Plugin;
	class PluginMetadata;
	class Identifier;

	class PluginPool
	{
	
	public:

		PluginPool(Engine &engine);
		virtual ~PluginPool();

		/**
		*	installs plugin
		*/
		const unsigned int install(std::string const& name, std::string const& dir, std::string const& file, std::string const& classname);

		/**
		*
		*/
		const bool contains(Identifier const& id) const;

		/**
		*	uninstalls plugin
		*/
		void uninstall(Identifier const& id);

		/**
		*	returns plugin pointer
		*/
		Plugin *const plugin(Identifier const& id);

		/**
		*	returns plugin pointer
		*/
		Plugin const *const plugin(Identifier const& id) const;

	private:

		typedef std::pair< unsigned int, Plugin * >			NamedPlugin;
		typedef std::map< unsigned int, Plugin * >			PluginMap;

		/**
		*	installed plugins
		*/
		PluginMap											m_Plugins;

		/**
		*	the 2 real engine
		*/
		Engine												&m_Engine;

	};

	class PluginNotFoundException : public Exception
	{

		friend class PluginPool;
		
		PluginNotFoundException(std::string const& name) :
			Exception(std::string("plugin pool does not contain ").append(name))
		{
		}

	};

}