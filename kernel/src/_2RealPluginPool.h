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
	class Data;
	class Runnable;
	class EngineData;

	typedef Poco::ClassLoader< IPluginActivator >								PluginLoader;
	typedef std::pair< std::string, Poco::ClassLoader< IPluginActivator > * >	NamedLibrary;
	typedef std::map< std::string, Poco::ClassLoader< IPluginActivator > * >	LibraryMap;
	typedef std::pair< Identifier, Plugin * >									NamedPlugin;
	typedef std::map< Identifier, Plugin * >									PluginMap;
	typedef std::pair< std::string, Metadata * >								NamedMetadata;
	typedef std::map< std::string, Metadata * >									MetadataMap;

	class PluginPool
	{
	
	public:

		PluginPool();
		~PluginPool();

		void							clear();
		void							setBaseDirectory(Poco::Path const& path);
		const std::list< std::string >	loadLibrary(Poco::Path const& path);
		const bool						isLibraryLoaded(Poco::Path const& path) const;

		//const Identifier		install(std::string const& idName, std::string const& className, Poco::Path const& path = Poco::Path());
		//bool					contains(Identifier const& id) const;
		//void					uninstall(Identifier const& id);
		//void					setup(Identifier const& id);
		const std::string				getInfoString(std::string const& className, Poco::Path const& path) const;
		Runnable &						createService(std::string const& name, Identifier const& id, std::string const& service);
		//void					setParameterValue(Identifier const& id, std::string const& paramName, Data const& data);
		//EngineData const&		getParameterValue(Identifier const& id, std::string const& paramName) const;
		//std::string const&		getParameterKey(Identifier const& id, std::string const& paramName) const;

	private:

		//bool					isLibraryLoaded(std::string const& classname) const;
		//Metadata const* const	getMetadata(std::string const& classname) const;
		//void					loadLibrary(std::string const& classname, Poco::Path const& filepath);

		Plugin &				getPlugin(Identifier const& id);
		Plugin const&			getPlugin(Identifier const& id) const;

		/**
		*	the poco classloader
		*/
		PluginLoader			m_PluginLoader;

		/*
		*	concrete instances of plugins
		*/
		PluginMap				m_Plugins;

		Poco::Path				m_BaseDirectory;

	};

}