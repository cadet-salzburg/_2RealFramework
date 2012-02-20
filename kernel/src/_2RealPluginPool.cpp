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

	PluginPool::PluginPool() :
		m_Plugins(),
		m_BaseDirectory(Poco::Path(""))
	{
	}

	PluginPool::~PluginPool()
	{
	}

	void PluginPool::setBaseDirectory(Poco::Path const& path)
	{
		if (path.isFile())
		{
			//TODO exc
		}

		m_BaseDirectory = path;
	}
	
	bool PluginPool::isLibraryLoaded(Poco::Path const& path) const
	{
		return false;
	}

	const std::string PluginPool::getInfoString(std::string const& className, std::string) const
	{
		return std::string();
	}

	const std::list< std::string > PluginPool::loadLibrary(Poco::Path const& path)
	{
		if (!path.isFile())
		{
			//ARGH
		}

		std::list< std::string > result;

		Poco::Path tmp;

		if (path.isAbsolute())
		{
			tmp = path;
		}
		else
		{
			tmp = m_BaseDirectory;
			tmp.append(path);
		}
		
		std::cout << tmp.toString() << std::endl;
		m_PluginLoader.loadLibrary(tmp.toString());
		const PluginLoader::Manif *manifest = m_PluginLoader.findManifest(tmp.toString());

		for (PluginLoader::Manif::Iterator it = manifest->begin(); it != manifest->end(); ++it)
		{
			std::string name = it->name();
			result.push_back(name);
		}
	}

	const Identifier createPlugin(std::string const& idName, std::string const& className, Poco::Path const& path)
	{
	}

	////const bool PluginPool::isSingleton(std::string const& classname) const
	////{
	////}

	////const Identifier PluginPool::install(std::string const& name, std::string const& classname)
	////{
	//////	
	////}

	void PluginPool::clear()
	{
		//for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); ++it)
		//{
		//	try
		//	{
		//		Identifier id = it->first;
		//		//it->second->uninstall();
		//		delete it->second;
		//	}
		//	catch (std::exception &e)
		//	{
		//		m_System.getLogstream() << "error on plugin uninstall: " << e.what();
		//	}
		//}

	//	m_Plugins.clear();
	}

	////bool PluginPool::contains(Identifier const& id) const
	////{
	////	return m_Plugins.find(id) != m_Plugins.end();
	////}

	////void PluginPool::uninstall(Identifier const& id)
	////{
	////	getPlugin(id).uninstall();
	////}

	////void PluginPool::setup(Identifier const& id)
	////{
	////	getPlugin(id).setup();
	////}

	//const std::string PluginPool::getInfoString(std::string const& classname) const
	//{
	//	return std::string(); //getPlugin(id).getInfoString();
	//}

	Runnable & PluginPool::createService(std::string const& name, Identifier const& id, std::string const& service)
	{
		return getPlugin(id).createService(name, service);
	}

	////void PluginPool::setParameterValue(Identifier const& id, std::string const& paramName, Data const& data)
	////{
	////	getPlugin(id).setParameterValue(paramName, data);
	////}

	////EngineData const& PluginPool::getParameterValue(Identifier const& id, std::string const& paramName) const
	////{
	////	return getPlugin(id).getParameterValue(paramName);
	////}

	////std::string const& PluginPool::getParameterKey(Identifier const& id, std::string const& paramName) const
	////{
	////	return getPlugin(id).getParameterKey(paramName);
	////}

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