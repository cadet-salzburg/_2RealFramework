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

	class Plugin;
	class Identifier;
	class SystemGraph;

	typedef std::pair< Identifier, Plugin * >	NamedPlugin;
	typedef std::map< Identifier, Plugin * >	PluginMap;

	class PluginPool
	{
	
	public:

		PluginPool(SystemGraph const& system);
		~PluginPool();

		const unsigned int		size() const;
		SystemGraph const&		system() const;
		void					setPluginDirectory(std::string const& directory);
		const Identifier		install(std::string const& name, std::string const& classname);
		const bool				contains(Identifier const& id) const;
		void					uninstall(Identifier const& id);
		Plugin &				getPlugin(Identifier const& id);
		Plugin const&			getPlugin(Identifier const& id) const;

	private:

		PluginMap				m_Plugins;
		SystemGraph				const& m_System;
		std::string				m_PluginDirectory;

	};

	inline const unsigned int PluginPool::size() const
	{
		return m_Plugins.size();
	}

	inline SystemGraph const& PluginPool::system() const
	{
		return m_System;
	}

	inline void PluginPool::setPluginDirectory(std::string const& directory)
	{
		m_PluginDirectory = directory;
	}

}