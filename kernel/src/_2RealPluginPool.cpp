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
#include "_2RealException.h"
#include "_2RealEntities.h"
#include "_2RealIdentifier.h"

namespace _2Real
{

	PluginPool::PluginPool() : m_Factory(NULL), m_Entities(NULL)
	{
	}

	PluginPool::PluginPool(PluginPool const& _src)
	{
		throw Exception::noCopy();
	}

	PluginPool& PluginPool::operator=(PluginPool const& _src)
	{
		throw Exception::noCopy();
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
		{
			try
			{
				it->second->uninstall();
				m_Entities->destroy(it->first);
				it->second = NULL;
			}
			catch (...)
			{
				std::cout << "plugin uninstall error: " << it->first << std::endl;
			}
		}
	}

	const Identifier PluginPool::install(std::string const& _name, std::string const& _path, std::string const& _class, std::list< Identifier > &_ids) throw(...)
	{
		try
		{
			const Entities::ID id = m_Entities->createPlugin(_name, _path, _class);
			Plugin *plugin = static_cast< Plugin * >(id.second);
			m_Plugins.insert(NamedPlugin(plugin->id(), plugin));
			plugin->install();
			plugin->load();
			plugin->start(_ids);
			return id.first;
		}
		catch (...)
		{
			throw Exception::failure();
		}
	}

	//void PluginPool::uninstall(unsigned int const& _id) throw(...)
	//{
	//	PluginMap::iterator it = m_Plugins.find(_id);

	//	if (it == m_Plugins.end())
	//	{
	//		throw Exception::failure();
	//	}

	//	try
	//	{
	//		it->second->uninstall();
	//		delete it->second;
	//		it->second = NULL;
	//	}
	//	catch (...)
	//	{
	//		throw Exception::failure();
	//	}
	//}

	//Plugin const *const PluginPool::get(unsigned int const& _id) const
	//{
	//	PluginMap::const_iterator it = m_Plugins.find(_id);
	//	
	//	if (it != m_Plugins.end())
	//	{
	//		return it->second;
	//	}

	//	return NULL;
	//}

}