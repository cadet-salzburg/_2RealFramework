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
#include "_2RealSystemGraph.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	PluginPool::PluginPool(SystemGraph &system) :
		m_Plugins(),
		m_System(system),
		m_InstallDirectory("")
	{
	}

	PluginPool::~PluginPool()
	{
		m_Plugins.clear();
	}

	const Identifier PluginPool::install(std::string const& name, std::string const& classname)
	{

		//for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
		//{
		//	if (it->second->getClassname() == classname)
		//	{
		//		std::ostringstream msg;
		//		msg << "plugin " << " " << classname << " is already loaded in system " << m_System.name() << std::endl;
		//		throw AlreadyExistsException(msg.str());
		//	}
		//}

		const Identifier id = Entity::createIdentifier(name, "plugin");

//TODO: dlls for other OS
#ifdef _DEBUG
		std::string file = classname + "_d.dll";
#else
		std::string file = classname + ".dll";
#endif

		m_System.getLogstream() << "creating plugin" << std::endl;

		Plugin *plugin = new Plugin(id, m_InstallDirectory, file, classname, m_System);

		m_System.getLogstream() << "plugin created" << std::endl;

		plugin->install();

		m_System.getLogstream() << "plugin installed" << std::endl;

		m_Plugins.insert(NamedPlugin(id, plugin));

		m_System.getLogstream() << "plugin inserted" << std::endl;

		return id;
	}

	void PluginPool::clearPlugins()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			try
			{
				Identifier id = it->first;
				//it->second->uninstall();
				delete it->second;
			}
			catch (std::exception &e)
			{
				m_System.getLogstream() << "error on plugin uninstall: " << e.what();
			}
		}

		m_Plugins.clear();
	}

	bool PluginPool::contains(Identifier const& id) const
	{
		return m_Plugins.find(id) != m_Plugins.end();
	}

	void PluginPool::uninstall(Identifier const& id)
	{
		getPlugin(id).uninstall();
	}

	void PluginPool::setup(Identifier const& id)
	{
		getPlugin(id).setup();
	}

	const std::string PluginPool::getInfoString(Identifier const& id) const
	{
		return getPlugin(id).getInfoString();
	}

	const Identifier PluginPool::createService(std::string const& name, Identifier const& id, std::string const& service)
	{
		return getPlugin(id).createService(name, service);
	}

	void PluginPool::setParameterValue(Identifier const& id, std::string const& paramName, EngineData const& value)
	{
		getPlugin(id).setParameterValue(paramName, value);
	}

	Plugin & PluginPool::getPlugin(Identifier const& id)
	{
		PluginMap::iterator it = m_Plugins.find(id);
		
		if (it == m_Plugins.end())
		{
			std::ostringstream msg;
			msg << "plugin " << id.name() << " not found in plugin pool";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}

	Plugin const& PluginPool::getPlugin(Identifier const& id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(id);

		if (it == m_Plugins.end())
		{
			std::ostringstream msg;
			msg << "plugin " << id.name() << " not found in plugin pool";
			throw NotFoundException(msg.str());
		}

		return *(it->second);
	}
}