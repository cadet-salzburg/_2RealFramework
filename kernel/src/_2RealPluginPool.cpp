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
#include "_2RealErrorState.h"
#include "_2RealIdentifierImpl.h"
#include "_2RealIdentities.h"

namespace _2Real
{

	PluginPool::PluginPool() : m_Factory(NULL), m_IDs(NULL)
	{
	}

	PluginPool::PluginPool(ServiceFactory *const _factory, Identities *const _ids) : m_Factory(_factory), m_IDs(_ids)
	{
	}

	PluginPool::PluginPool(PluginPool const& _src) throw(...)
	{
		throw ErrorState::failure();
	}

	PluginPool& PluginPool::operator=(PluginPool const& _src) throw(...)
	{
		throw ErrorState::failure();
	}

	PluginPool::~PluginPool()
	{
		for (PluginMap::iterator it = m_Plugins.begin(); it != m_Plugins.end(); it++)
		{
			try
			{
				it->second->uninstall();
				delete it->second;
				it->second = NULL;
			}
			catch (...)
			{
				std::cout << "plugin uninstall error: " << it->first.name() << std::endl;
			}
		}
	}

	IdentifierImpl const *const PluginPool::install(std::string const& _name, std::string const& _path, std::string const& _class) throw(...)
	{
		try
		{
			const IdentifierImpl *id = m_IDs->createID(_name, IdentifierImpl::PLUGIN);
			Plugin *plugin = new Plugin(_path, _class, m_Factory);
			m_Plugins.insert(NamedPlugin(*id, plugin));
			plugin->install(*id);
			plugin->load();
			plugin->start();
			return id;
		}
		catch (...)
		{
			throw ErrorState::failure();
		}
	}

	void PluginPool::uninstall(IdentifierImpl const& _id) throw(...)
	{
		PluginMap::iterator it = m_Plugins.find(_id);

		if (it == m_Plugins.end())
		{
			throw ErrorState::failure();
		}

		try
		{
			it->second->uninstall();
			delete it->second;
			it->second = NULL;
		}
		catch (...)
		{
			throw ErrorState::failure();
		}
	}

	Plugin const *const PluginPool::get(IdentifierImpl const& _id) const
	{
		PluginMap::const_iterator it = m_Plugins.find(_id);
		
		if (it != m_Plugins.end())
		{
			return it->second;
		}

		return NULL;
	}

}