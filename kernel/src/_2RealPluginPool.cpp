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

#include "_2RealPluginPool.h"
#include "_2RealPlugin.h"
#include "_2RealPluginMetadata.h"
#include "_2RealEntityTable.h"
#include "_2RealIdentifier.h"
#include "_2RealSetupParameter.h"

#include <iostream>

namespace _2Real
{

	PluginPool::PluginPool(Engine &_engine) :
		m_Plugins(),
		m_Engine(_engine)
	{
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
		{
			try
			{
				it->second->uninstall();
				m_Engine.entities().destroy(it->second);
			}
			catch (Exception &e)
			{
				std::cout << "error on plugin pool destruction: " << e.what() << std::endl;
			}
		}
	}

	const unsigned int PluginPool::install(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{
		try
		{
			Plugin *plugin = m_Engine.entities().createPlugin(_name, _dir, _file, _class);
			plugin->install();

			const PluginMetadata data = plugin->pluginMetadata();
			typedef std::map< std::string, std::string > StringMap;
			StringMap setup = data.getSetupParameters();
			for (StringMap::iterator it = setup.begin(); it != setup.end(); it++)
			{
				SetupParameter *param = m_Engine.entities().createSetupParameter(it->first, it->second, plugin);
				plugin->addSetupParameter(param);
			}

			m_Plugins.insert(NamedPlugin(plugin->id(), plugin));
			
			return plugin->id();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	const bool PluginPool::contains(Identifier const& _id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(_id.id());
		return (it != m_Plugins.end());
	}

	void PluginPool::uninstall(Identifier const& _id)
	{
		plugin(_id)->uninstall();
	}

	Plugin const *const PluginPool::plugin(Identifier const& _id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(_id.id());
		
		if (it == m_Plugins.end())
		{
			throw PluginNotFoundException(_id.name());
		}

		return it->second;
	}

	Plugin *const PluginPool::plugin(Identifier const& _id)
	{
		PluginMap::iterator it = m_Plugins.find(_id.id());
		
		if (it == m_Plugins.end())
		{
			throw PluginNotFoundException(_id.name());
		}

		return it->second;
	}
}