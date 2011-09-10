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

#include "_2RealPluginMetadata.h"
#include "_2RealPluginPool.h"
#include "_2RealPlugin.h"
#include "_2RealException.h"
#include "_2RealEntities.h"

#include <iostream>

namespace _2Real
{

	PluginPool::PluginPool() : m_Factory(NULL), m_Entities(NULL)
	{
	}

	PluginPool::PluginPool(PluginPool const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	PluginPool& PluginPool::operator=(PluginPool const& _src)
	{
		throw Exception("attempted to copy entity");
	}

	PluginPool::~PluginPool()
	{
		try
		{
			for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
			{
				it->second->uninstall();
				m_Entities->destroy(it->first);
			}

			m_Plugins.clear();
		}
		catch (Exception &e)
		{
			std::cout << "error on plugin pool destruction " << e.what() << std::endl;
		}
	}

	const unsigned int PluginPool::install(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{
		try
		{
			const EntityTable::ID id = m_Entities->createPlugin(_name, _dir, _file, _class);
			Plugin *plugin = static_cast< Plugin * >(id.second);
			m_Plugins.insert(NamedPlugin(id.first, plugin));

			plugin->install();
			plugin->load();
			plugin->start();

			return id.first;
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Plugin const *const PluginPool::getPlugin(unsigned int const& _id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(_id);
		if (it == m_Plugins.end())
		{
			throw Exception("could not retrieve plugin - does not exist");
		}
		else if (!it->second)
		{
			throw Exception("internal error - null pointer stored in plugin pool");
		}
		return it->second;
	}

	Plugin *const PluginPool::getPlugin(unsigned int const& _id)
	{
		PluginMap::iterator it = m_Plugins.find(_id);
		if (it == m_Plugins.end())
		{
			throw Exception("could not retrieve plugin - does not exist");
		}
		else if (!it->second)
		{
			throw Exception("internal error - null pointer in plugin pool");
		}
		return it->second;
	}

	PluginMetadata const& PluginPool::pluginInfo(unsigned int const& _id) const
	{
		try
		{
			return getPlugin(_id)->metadata();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}