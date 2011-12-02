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
#include "_2RealTypes.h"
#include "_2RealSetupParameter.h"
#include "_2RealSystemGraph.h"

#include <sstream>

namespace _2Real
{

	PluginPool::PluginPool(SystemGraph const& system) :
		m_Plugins(),
		m_System(system)
	{
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			try
			{
				Identifier id = it->first;
				Engine::instance()->getLogStream() << "plugin pool: uninstalling " << id << "\n";
			
				it->second->uninstall();
				delete it->second;
			}
			catch (_2Real::Exception &e)
			{
				Engine::instance()->getLogStream() << "plugin pool: error on plugin destruction " << e.what() << "\n";
			}
			catch (...)
			{
				Engine::instance()->getLogStream() << "plugin pool: error on plugin destruction\n";
			}
		}

		m_Plugins.clear();
	}

	const Identifier PluginPool::install(std::string const& name, std::string const& directory, std::string const& file, std::string const& classname)
	{
		_2Real::Engine *engine = Engine::instance();
		engine->getLogStream() << "plugin pool: installing\n";
		engine->getLogStream() << name << " " << directory << " " << file << " " << classname << "\n";
		engine->getLogStream() << "plugin pool: initial size: " << m_Plugins.size() << "\n";

		const Identifier id = Entity::createIdentifier(name, "plugin");
		Plugin *plugin = new Plugin(id, directory, file, classname, m_System);
		engine->getLogStream() << "plugin pool: installing now\n";
		plugin->install();
		engine->getLogStream() << "plugin pool: installed sucessfully, creating setup parameters\n";

		ParameterMap const& setup = plugin->setupParameters();
		for (ParameterMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
		{
			const Identifier id = Entity::createIdentifier(it->first, "plugin setup parameter");
			std::string const& keyword = it->second->datatype();
			std::string const& type = engine->types().getTypename(keyword);
			engine->getLogStream() << "plugin pool: creating setup parameter " << it->first << " " << keyword << " " << type << "\n";
			SetupParameter *param = new SetupParameter(id, type, keyword);
			plugin->addSetupParameter(param);
		}

		//engine->getLogStream() << "plugin pool: installed succesfully, uninstalling now\n";
		//plugin->uninstall();
		//engine->getLogStream() << "plugin pool: uninstalled successfully\n";
		//delete plugin;

		m_Plugins.insert(NamedPlugin(id, plugin));
		engine->getLogStream() << "plugin pool: new size: " << m_Plugins.size() << "\n";
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