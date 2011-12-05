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
#include "_2RealIdentifier.h"
#include "_2RealTypetable.h"
#include "_2RealSetupParameter.h"
#include "_2RealSystemGraph.h"

#include <sstream>

namespace _2Real
{

	PluginPool::PluginPool(SystemGraph const& system) :
		m_Plugins(),
		m_System(system),
		//initially, plugins are searched within the execution directory
		m_PluginDirectory("")
	{
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			try
			{
				Identifier id = it->first;
				it->second->uninstall();
				delete it->second;
			}
			catch (_2Real::Exception &e)
			{
				//std::cout << e.what() << std::endl;
			}
			catch (...)
			{
			}
		}

		m_Plugins.clear();
	}

	const Identifier PluginPool::install(std::string const& name, std::string const& classname)
	{
		const Identifier id = Entity::createIdentifier(name, "plugin");

/**
*	currently, assumes that plugins end with _d in debug mode, otherwise classname = dll name
*/
#ifdef _DEBUG
		std::string file = classname + "_d.dll";
#else
		std::string file = classname + ".dll";
#endif

		//std::cout << "installing: " << m_PluginDirectory << file << std::endl;

		Plugin *plugin = new Plugin(id, m_PluginDirectory, file, classname, m_System);

		//std::cout << "installing now " << file << std::endl;

		plugin->install();
		m_Plugins.insert(NamedPlugin(id, plugin));

		return id;
	}

	const bool PluginPool::contains(Identifier const& id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(id);
		return (it != m_Plugins.end());
	}

	void PluginPool::uninstall(Identifier const& id)
	{
		getPlugin(id).uninstall();
	}

	Plugin & PluginPool::getPlugin(Identifier const& id)
	{
		PluginMap::iterator it = m_Plugins.find(id);
		
		if (it == m_Plugins.end())
		{
			std::ostringstream msg;
			msg << "internal error: plugin " << id.name() << " not found in plugin pool";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}

	Plugin const& PluginPool::getPlugin(Identifier const& id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(id);

		if (it == m_Plugins.end())
		{
			std::ostringstream msg;
			msg << "internal error: plugin " << id.name() << " not found in plugin pool";
			throw _2Real::Exception(msg.str());
		}

		return *(it->second);
	}
}