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
#include "_2RealTypes.h"
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
			std::string name;
			try
			{
				name = it->second->name();
				std::cout << "deleting plugin: " << name << std::endl;
				it->second->uninstall();
				delete it->second;
			}
			catch (Exception &e)
			{
				std::cout << e.what() << std::endl;
			}
			catch (...)
			{
				std::cout << "error on plugin destruction: " << name << std::endl;
			}
		}
	}

	const Identifier PluginPool::install(std::string const& _name, std::string const& _dir, std::string const& _file, std::string const& _class)
	{
		try
		{
			const Identifier id = m_Engine.entities().createIdentifier(_name, "plugin");

			PluginMetadata *data = new PluginMetadata(_class, _file, _dir, m_Engine.types());
			Plugin *plugin = new Plugin(id, data);
			plugin->install();

			typedef std::map< std::string, std::string > StringMap;
			StringMap setup = data->getSetupParameters();
			for (StringMap::iterator it = setup.begin(); it != setup.end(); it++)
			{
				const Identifier id = m_Engine.entities().createIdentifier(it->first, "plugin setup parameter");
				SetupParameter *param = new SetupParameter(id, m_Engine.types().getTypename(it->second), it->second);
				plugin->addSetupParameter(param);
			}

			m_Plugins.insert(NamedPlugin(plugin->id(), plugin));
			
			return id;
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