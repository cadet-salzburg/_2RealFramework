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

	PluginPool::PluginPool() :
		m_Entities(NULL),
		m_Factory(NULL)
	{
	}

	PluginPool::PluginPool(PluginPool const& _src)
	{
		throw Exception("internal error: attempted to copy an entity");
	}

	PluginPool& PluginPool::operator=(PluginPool const& _src)
	{
		throw Exception("internal error: attempted to copy an entity");
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
		{
			try
			{
				unsigned int id = it->first;
				uninstall(id);
				m_Entities->destroy(it->second);
			}
			catch (Exception &e)
			{
				std::cout << "error on plugin pool destruction " << e.what() << std::endl;
			}
		}
	}

	const unsigned int PluginPool::install(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{
		try
		{

			Plugin *plugin = m_Entities->createPlugin(_name, _dir, _file, _class);

#ifdef _VERBOSE
		std::cout << "plugin pool: installing dll " << _dir << " " << _file << std::endl;
#endif

			plugin->install(m_Factory);
			m_Plugins.insert(NamedPlugin(plugin->id(), plugin));

#ifdef _VERBOSE
		std::cout << "plugin pool: success" << std::endl;
#endif
			return plugin->id();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	void PluginPool::uninstall(unsigned int const& _id)
	{
		try
		{
			plugin(_id)->uninstall();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}

	Plugin const *const PluginPool::plugin(unsigned int const& _id) const
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

	Plugin *const PluginPool::plugin(unsigned int const& _id)
	{
		PluginMap::iterator it = m_Plugins.find(_id);
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

	PluginMetadata const& PluginPool::info(unsigned int const& _id) const
	{
		try
		{
			return plugin(_id)->pluginMetadata();
		}
		catch (Exception &e)
		{
			throw e;
		}
	}
}