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

#include "_2RealPlugin.h"
#include "_2RealPluginContext.h"

namespace _2Real
{

	Plugin::Plugin(std::string const& _name, std::string const& _path, Framework &_context) :
		m_PluginState(Plugin::INVALID), m_PluginName(_name), m_ActivatorPtr(NULL), m_Framework(_context)
	{
#ifdef _DEBUG
		m_LibraryPath = _path+_name+"_d";
#else
		m_LibraryPath = _path+_name;
#endif

		m_ContextPtr = new PluginContext(this);

		m_PluginState = Plugin::INSTALLED;
	}

	Plugin::ePluginState const& Plugin::state() const
	{
		return m_PluginState;
	}
		
	std::string const& Plugin::name() const
	{
		return m_PluginName;
	}

	std::string const& Plugin::path() const
	{
		return m_LibraryPath;
	}

	const Metadata *const Plugin::metadata() const
	{
		return m_MetadataPtr;
	}

	void Plugin::registerService(std::string const& _name, ServiceCreator _creator, bool const& _singleton) const
	{
		m_Framework.registerService(_name, m_PluginName, _creator, _singleton);
	}

	void Plugin::load()
	{
		if (m_PluginState == Plugin::INSTALLED)
		{

			try
			{
				m_PluginLoader.loadLibrary(m_LibraryPath);
			}
			catch (...)
			{
				std::cout << "TODO: error handling; plugin::load" << std::endl;
				m_PluginState = Plugin::INVALID;
				return;
			}

			m_PluginState = Plugin::LOADED;
		}
	}

	void Plugin::start()
	{
		if (m_PluginState == Plugin::LOADED)
		{
			try
			{
				if (m_PluginLoader.canCreate(m_PluginName))
				{
					m_ActivatorPtr = m_PluginLoader.create(m_PluginName);
					if (!m_ActivatorPtr)
					{
						std::cout << "TODO: error handling; plugin::start" << std::endl;
						m_PluginState = Plugin::INVALID;
						return;
					}

					m_ActivatorPtr->start(m_ContextPtr);
				}
				else
				{
					std::cout << "TODO: error handling; plugin::start" << std::endl;
					m_PluginState = Plugin::INVALID;
					return;
				}
			}
			catch (...)
			{
				if (m_ActivatorPtr)
				{
					delete m_ActivatorPtr;
				}

				std::cout << "TODO: error handling; plugin::start" << std::endl;
				m_PluginState = Plugin::INVALID;
				return;
			}

			m_PluginState = Plugin::ACTIVE;
		}
	}

	void Plugin::stop()
	{
		if (m_PluginState == Plugin::ACTIVE)
		{
			try
			{
				//TODO::invalidate all services
				m_PluginLoader.destroy(m_PluginName, m_ActivatorPtr);
			}
			catch (...)
			{
				std::cout << "TODO: error handling; plugin::stop" << std::endl;
				m_PluginState = Plugin::INVALID;
				return;
			}

			delete m_ActivatorPtr;
			m_ActivatorPtr = NULL;
			m_PluginState = Plugin::LOADED;
		}
	}

	void Plugin::unload()
	{
		if (m_PluginState == Plugin::LOADED)
		{
			try
			{
				if (m_PluginLoader.isLibraryLoaded(m_LibraryPath))
				{
					m_PluginLoader.unloadLibrary(m_LibraryPath);
				}
			}
			catch(...)
			{
				std::cout << "TODO: error handling; plugin::stop" << std::endl;
				m_PluginState = Plugin::INVALID;
				return;
			}

			m_PluginState = Plugin::INSTALLED;
		}
	}

}