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
#include "_2RealMetadata.h"

#include <iostream>
#include <sstream>

namespace _2Real
{

	PluginPool::PluginPool() :
		m_Plugins(),
		m_Metadata(),
		m_PluginLoader(),
		m_Names(),
		m_BaseDirectory(Poco::Path())
	{
	}

	PluginPool::~PluginPool()
	{
		clear();
	}

	void PluginPool::setBaseDirectory(Poco::Path const& path)
	{
		m_BaseDirectory = path;
	}
	
	const bool PluginPool::isLibraryLoaded(Poco::Path const& path) const
	{
		Poco::Path tmp = makeAbsolutePath(path);
		return m_PluginLoader.isLibraryLoaded(tmp.toString());
	}

	const std::string PluginPool::getInfoString(std::string const& className, Poco::Path const& path) const
	{
		Poco::Path tmp = makeAbsolutePath(path);
		std::string metaKey = pathToName(tmp) + "." + toLower(className);

		if (!isLibraryLoaded(path))
		{
			throw NotFoundException("library " + path.toString() + " is not loaded");
		}

		MetadataMap::const_iterator it = m_Metadata.find(metaKey);
		if (it == m_Metadata.end())
		{
			throw NotFoundException("library " + path.toString() + " exports no class called " + className);
		}

		std::ostringstream s;
		s << *it->second;
		return s.str();
	}

	const std::string PluginPool::getInfoString(Identifier const& pluginId) const
	{
		Plugin const& plugin = getPlugin(pluginId);
		return plugin.getInfoString();
	}

	const std::list< std::string > PluginPool::loadLibrary(Poco::Path const& path)
	{
		std::list< std::string > result;

		Poco::Path tmp = this->makeAbsolutePath(path);

		std::cout << "loading library: " << tmp.toString() << std::endl;

		m_PluginLoader.loadLibrary(tmp.toString());

		std::cout << "loaded library: " << tmp.toString() << std::endl;

		const PluginLoader::Manif *manifest = m_PluginLoader.findManifest(tmp.toString());
		for (PluginLoader::Manif::Iterator it = manifest->begin(); it != manifest->end(); ++it)
		{
			std::string className = it->name();
			std::cout << "found manifest " << className << std::endl;

			PluginMetadata *pluginMeta = new PluginMetadata(className, tmp.toString());
			Metadata meta(*pluginMeta);

			if (it->canCreate())
			//create a temporary plugin activator & get the metadata
			{
				std::cout << "found a normal plugin " << className << std::endl;
				IPluginActivator *dummy = it->create();
				std::cout << "created an instance " << className << std::endl;
				dummy->getMetadata(meta);
				it->destroy(dummy);
				std::cout << "destroyed dummy instance " << className << std::endl;
				delete dummy;
				std::cout << "deleted dummy instance " << className << std::endl;
			}
			else
			//it's a singleton -> create an identifier & store the plugin
			{
				std::cout << "found a singleton plugin " << className << std::endl;
				IPluginActivator &instance = it->instance();
				instance.getMetadata(meta);

				std::string idName = pathToName(tmp) + "." + toLower(className);
				const Identifier id = Entity::createIdentifier(idName, "plugin");
				Plugin *plugin = new Plugin(id, instance, *pluginMeta);
				m_Plugins.insert(NamedPlugin(id, plugin));
				m_Names.insert(make_pair(idName, id));
				std::cout << "saved singleton instance " << id << std::endl;
			}

			std::string metaKey = pathToName(tmp) + "." + toLower(className);
			m_Metadata.insert(NamedMetadata(metaKey, pluginMeta));
			
			std::cout << "loaded class: " << className << std::endl;
			result.push_back(className);
		}

		return result;
	}

	const Identifier PluginPool::createInstance(std::string const& className, Poco::Path const& path)
	{
		unsigned int counter = 0;
		//this could be done more efficiently, currently i just count the instances of a particular plugin
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			Plugin *p = it->second;
			if (p->getClassName() == className && p->getLibraryPath() == path.toString())
			{
				counter++;
			}
		}

		std::ostringstream s;
		s << counter;

		Poco::Path tmp = makeAbsolutePath(path);
		std::string idName = pathToName(tmp) + "." + toLower(className) + " nr. " + s.str();

		return createInstance(idName, className, path);
	}

	const Identifier PluginPool::createInstance(std::string const& idName, std::string const& className, Poco::Path const& path)
	{
		Poco::Path tmp = makeAbsolutePath(path);

		std::string name = validateName(idName);
		if (!isUnique(name))
		{
			throw InvalidNameException("plugin name " + name + " must be unique");
		}

		const PluginLoader::Manif *manifest = m_PluginLoader.findManifest(tmp.toString());
		if (!manifest)
		{
			throw NotFoundException("the library " + tmp.toString() + " is either not loaded or exports no manifest");
		}

		std::string metaKey = pathToName(tmp) + "." + toLower(className);
		MetadataMap::iterator meta = m_Metadata.find(metaKey);
		if (meta == m_Metadata.end())
		{
			throw NotFoundException("no class called " + className + " is exported by " + tmp.toString());
		}

		PluginLoader::Manif::Iterator it = manifest->find(className);
		if (it == manifest->end())
		{
			throw NotFoundException("no class called " + className + " is exported by " + tmp.toString());
		}

		if (it->canCreate())
		{
			IPluginActivator *activator = it->create();
			it->autoDelete(activator);
			const Identifier id = Entity::createIdentifier(name, "plugin");
			Plugin *plugin = new Plugin(id, *activator, *meta->second);
			m_Plugins.insert(NamedPlugin(id, plugin));

			return id;
		}
		else
		{
			throw InvalidAccessException("plugin class " + className + " is a singleton");
		}
	}

	const Identifier PluginPool::getInstance(std::string const& className, Poco::Path const& path)
	{
		if (!isSingleton(className, path))
		{
			throw InvalidAccessException("plugin class " + className + " is no singleton");
		}

		Poco::Path tmp = makeAbsolutePath(path);
		std::string singletonName = pathToName(tmp) + "." + toLower(className);
		std::cout << singletonName << std::endl;

		std::map< std::string, Identifier >::iterator it = m_Names.find(singletonName);
		if (it == m_Names.end())
		{
			for (std::map< std::string, Identifier >::iterator f = m_Names.begin(); f != m_Names.end(); ++f)
			{
				std::cout << f->first << std::endl;
			}
			throw NotFoundException("singleton not found");
		}

		return it->second;
	}

	const bool PluginPool::isSingleton(std::string const& className, Poco::Path const& path) const
	{
		Poco::Path tmp = makeAbsolutePath(path);

		const PluginLoader::Manif *manifest = m_PluginLoader.findManifest(tmp.toString());
		if (!manifest)
		{
			throw NotFoundException("the library " + tmp.toString() + " is either not loaded or exports no manifest");
		}

		PluginLoader::Manif::Iterator it = manifest->find(className);
		if (it == manifest->end())
		{
			throw NotFoundException("no class called " + className + " is exported by " + tmp.toString());
		}

		if (it->canCreate())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	const bool PluginPool::canCreate(std::string const& className, Poco::Path const& path) const
	{
		return !isSingleton(className, path);
	}

	void PluginPool::clear()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		{
			Plugin *p = it->second;
			std::string libPath = p->getLibraryPath();
			std::string className = p->getClassName();

			//std::cout << "deleting plugin: " << libPath << " " << className << std::endl;

			if (!isSingleton(className, Poco::Path(libPath)))
			{
				//std::cout << "no singleton: " << libPath << " " << className << std::endl;
				IPluginActivator *activator = &(p->getActivator());
				const PluginLoader::Manif *manifest = m_PluginLoader.findManifest(libPath);
				PluginLoader::Manif::Iterator mIt = manifest->find(className);
				mIt->destroy(activator);
			}

			delete p;
		}

		m_Plugins.clear();

		for (MetadataMap::iterator it = m_Metadata.begin(); it != m_Metadata.end(); ++it)
		{
			PluginMetadata *m = it->second;
			//std::cout << "deleting metadata: " << m->getClassname() << std::endl;
			delete m;
		}

		//std::cout << "deleted all metadata" << std::endl;
		m_Metadata.clear();

		//WTF?
		std::list< std::string > tmp;
		for (PluginLoader::Iterator it = m_PluginLoader.begin(); it!=m_PluginLoader.end(); ++it)
		{
			std::string libPath = it->first;
			tmp.push_back(libPath);
		}

		for (std::list< std::string >::iterator it = tmp.begin(); it != tmp.end(); ++it)
		{
			std::cout << "unloading " << *it << std::endl;

			try
			{
				m_PluginLoader.unloadLibrary(*it);
			}
			catch (Poco::NotFoundException &e)
			{
				std::cout << "notfound" << std::endl;
			}
			catch (...)
			{
				std::cout << "ERROR" << std::endl;
			}

			std::cout << "unloaded!" << std::endl;
		}

		tmp.clear();
	}

	bool PluginPool::contains(Identifier const& id) const
	{
		return m_Plugins.find(id) != m_Plugins.end();
	}

	void PluginPool::setup(Identifier const& id)
	{
		getPlugin(id).setup();
	}

	const bool PluginPool::isSetUp(Identifier const& id) const
	{
		return getPlugin(id).isSetUp();
	}

	Runnable & PluginPool::createService(Identifier const& pluginId, std::string const& serviceName, SystemImpl &graph)
	{
		return getPlugin(pluginId).createService(serviceName, graph);
	}

	Runnable & PluginPool::createService(std::string const& idName, Identifier const& pluginId, std::string const& serviceName, SystemImpl &graph)
	{
		return getPlugin(pluginId).createService(idName, serviceName, graph);
	}

	void PluginPool::setParameterValue(Identifier const& id, std::string const& paramName, EngineData const& data)
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

	const Identifier PluginPool::getIdentifier(std::string const& idName) const
	{
		std::map< std::string, Identifier >::const_iterator it = m_Names.find(idName);
		if (it == m_Names.end())
		{
			throw NotFoundException("identifier named " + idName + " not found in plugin pool");
		}

		return it->second;
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

	const bool PluginPool::isUnique(std::string const& idName) const
	{
		return (m_Names.find(idName) == m_Names.end());
	}

	const Poco::Path PluginPool::makeAbsolutePath(Poco::Path const& path) const
	{
		if (path.isAbsolute())
		{
			return path;
		}
		else
		{
			Poco::Path tmp = m_BaseDirectory;
			tmp.append(path);
			return tmp;
		}
	}
}