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

#ifdef _2REAL_WINDOWS
	#ifndef _DEBUG
		#define shared_library_suffix ".dll"
	#else
		#define shared_library_suffix "_d.dll"
	#endif
#elif _2REAL_UNIX
	#ifndef _DEBUG
		#define shared_library_suffix ".so"
	#else
		#define shared_library_suffix "_d.so"
	#endif
#elif _2REAL_MAC
	#ifndef _DEBUG
		#define shared_library_suffix ".dylib"
	#else
		#define shared_library_suffix "_d.dylib"
	#endif
#endif

namespace _2Real
{

	PluginPool::PluginPool(SystemGraph &system) :
		m_PluginInstances(),
		m_System(system),
		m_InstallDirectory("")
	{
	}

	PluginPool::~PluginPool()
	{
		m_PluginInstances.clear();
	}

	void PluginPool::setInstallDirectory(std::string const& directory)
	{
		m_InstallDirectory = directory;
	}

	bool PluginPool::isLibraryLoaded(std::string const& classname) const
	{
		return (m_LoadedLibs.find(classname) != m_LoadedLibs.end());
	}

	const Identifier PluginPool::install(std::string const& name, std::string const& classname)
	{
		if (!isLibraryLoaded(classname))
		{
			loadLibrary
		}


		LibraryMap::iterator iter = m_LoadedLibs.find(classname);
		if (iter == m_LoadedLibs.end())
		{
			PluginLoader *loader = new PluginLoader();
			try
			{
				loader->loadLibrary(CLASSPATH);
			}
			catch (Poco::LibraryLoadException &e)
			{
				throw _2Real::Exception(e.what());
			}

			if (loader.canCreate(classname))
			{
				try
				{
					IPluginActivator *dummy = loader.create(classname);
				}
				catch (Poco::NotFoundException &e)
				{
					m_PluginLoader.unloadLibrary(CLASSPATH);
					throw Exception(e.what());
				}
			}
			else
			{
				try
				{
					IPluginActivator &instance = loader.instance(classname);
				}
				catch(Poco::InvalidAccessException &e)
				{
					m_PluginLoader.unloadLibrary(CLASSPATH);
					throw Exception(e.what());
				}
			}

			m_LoadedLibs.insert(NamedLibrary(classname, loader));
		}
		else
		{
			const Identifier id = Entity::createIdentifier(name, "plugin");

			PluginLoader *loader = iter->second;

			if (loader.canCreate(classname))
			{
				try
				{
					IPluginActivator *dummy = loader.create(classname);
					Plugin *plugin = newPlugin(
				}
				catch (Poco::NotFoundException &e)
				{
					throw Exception(e.what());
				}
			}
			else
			{
				try
				{
					IPluginActivator &instance = loader.instance(classname);
				}
				catch(Poco::InvalidAccessException &e)
				{
					throw Exception(e.what());
				}
			}
		}
				//3rd, read the metadata
				Metadata metadata(m_Metadata);
				m_Activator->getMetadata(metadata);

				//build setup params
				ParameterDataMap const& setup = m_Metadata.getSetupParameters();
				for (ParameterDataMap::const_iterator it = setup.begin(); it != setup.end(); ++it)
				{
					SetupParameter *setup = new SetupParameter(*it->second);
					m_SetupParameters.insert(NamedParameter(it->second->getName(), setup));
				}
			}
			catch (_2Real::Exception &e)
			{
				if (m_Activator)
				{
					m_PluginLoader.destroy(m_Metadata.getClassname(), m_Activator);
					m_Activator = NULL;
				}

				if (m_PluginLoader.isLibraryLoaded(m_File))
				{
					m_PluginLoader.unloadLibrary(m_File);
				}

				e.rethrow();
			}
		}

		const Identifier id = Entity::createIdentifier(name, "plugin");

		std::string file = classname + shared_library_suffix;

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

	Runnable & PluginPool::createService(std::string const& name, Identifier const& id, std::string const& service)
	{
		return getPlugin(id).createService(name, service);
	}

	void PluginPool::setParameterValue(Identifier const& id, std::string const& paramName, Data const& data)
	{
		getPlugin(id).setParameterValue(paramName, data);
	}

	EngineData const& PluginPool::getParameterValue(Identifier const& id, std::string const& paramName) const
	{
		return getPlugin(id).getParameterValue(paramName);
	}

	std::string const& PluginPool::getParameterKey(Identifier const& id, std::string const& paramName) const
	{
		return getPlugin(id).getParameterKey(paramName);
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