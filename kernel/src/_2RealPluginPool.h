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

#include "_2RealIPluginActivator.h"

#include <map>
#include <string>
#include <list>

#include "Poco/Path.h"
#include "Poco/ClassLoader.h"

namespace _2Real
{

	class Plugin;
	class Identifier;
	class SystemGraph;
	class Runnable;
	class EngineData;
	class PluginMetadata;

	typedef Poco::ClassLoader< IPluginActivator >								PluginLoader;
	typedef std::pair< std::string, Poco::ClassLoader< IPluginActivator > * >	NamedLibrary;
	typedef std::map< std::string, Poco::ClassLoader< IPluginActivator > * >	LibraryMap;
	typedef std::pair< Identifier, Plugin * >									NamedPlugin;
	typedef std::map< Identifier, Plugin * >									PluginMap;
	typedef std::pair< std::string, PluginMetadata * >							NamedMetadata;
	typedef std::map< std::string, PluginMetadata * >							MetadataMap;

	class PluginPool
	{
	
	public:

		PluginPool();
		~PluginPool();

		void								clear();
		void								setBaseDirectory(Poco::Path const& path);
		const std::list< std::string >		loadLibrary(Poco::Path const& path);
		const bool							isLibraryLoaded(Poco::Path const& path) const;
		const bool							canCreate(std::string const& className, Poco::Path const& path) const;
		const bool							isSingleton(std::string const& className, Poco::Path const& path) const;
		const bool							isSetUp(Identifier const& pluginId) const;
		const std::string					getInfoString(std::string const& className, Poco::Path const& path) const;
		const std::string					getInfoString(Identifier const& pluginId) const;
		Runnable &							createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName, SystemGraph &graph);
		const Identifier					createInstance(std::string const& idName, std::string const& className, Poco::Path const& path);
		const Identifier					getInstance(std::string const& className, Poco::Path const& path);
		void								setup(Identifier const& pluginId);
		const Identifier					getIdentifier(std::string const& idName) const;
		bool								contains(Identifier const& id) const;
		void								setParameterValue(Identifier const& id, std::string const& paramName, EngineData const& data);
		EngineData const&					getParameterValue(Identifier const& id, std::string const& paramName) const;
		std::string const&					getParameterKey(Identifier const& id, std::string const& paramName) const;

	private:

		const Poco::Path		makeAbsolutePath(Poco::Path const& path) const;

		Plugin &				getPlugin(Identifier const& id);
		Plugin const&			getPlugin(Identifier const& id) const;

		const bool				isUnique(std::string const& name) const;

		/**
		*	the poco classloader
		*/
		PluginLoader							m_PluginLoader;

		/*
		*	concrete instances of plugins
		*/
		PluginMap								m_Plugins;

		/**
		*	each exported plugin's metadata will be stored here
		*/
		MetadataMap								m_Metadata;

		/**
		*	the base directory for relative paths
		*/
		Poco::Path								m_BaseDirectory;

		/**
		*	a lookup table for all plugin names
		*/
		std::map< std::string, Identifier >		m_Names;

	};

}