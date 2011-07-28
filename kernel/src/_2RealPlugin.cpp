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
#include "_2RealContextPrivate.h"
#include "_2RealConfigMetadata.h"
#include "_2RealIService.h"

namespace _2Real
{
	Plugin::Plugin(const std::string& _name, const std::string& _dir, ContextPrivate* _context) : 
						m_PluginState(Plugin::UNINSTALLED), m_PluginName(_name), m_LibraryPath(_dir+_name), m_ContextPtr(_context), m_PluginActivator(NULL)
	{
		m_PluginContextPtr = PluginContextPtr(new PluginContext(this, _context));
		m_PluginState = Plugin::INSTALLED;
	}

	Plugin::~Plugin()
	{
		delete m_PluginContextPtr.assign(NULL);
	}

	const Plugin::PluginState Plugin::state() const
	{
		return m_PluginState;
	}
		
	const std::string Plugin::name() const
	{
		return m_PluginName;
	}

	const std::string Plugin::path() const
	{
		return m_LibraryPath;
	}

	MetadataPtr Plugin::metadata() const
	{
		if (m_PluginState == ACTIVE)
		{
			return m_PluginActivator->metadata();
		}

		return NULL;
	}

	void Plugin::load()
	{
		switch (m_PluginState)
		{
			case Plugin::UNINSTALLED:
			case Plugin::LOADED:
			case Plugin::ACTIVE:
				break;

			case Plugin::INSTALLED:
				try
				{
					loadLibrary();
				}
				catch (std::logic_error e)
				{
					std::cout << "Plugin::load() - could not load the dll " << e.what() << std::endl;
				}
				break;
		}
	}

	void Plugin::loadLibrary()
	{
		try
		{
			m_PluginLoader.loadLibrary(m_LibraryPath);
			m_PluginState = Plugin::LOADED;
		}
		catch (Poco::Exception& e)
		{
			std::string msg = "Plugin::loadLibrary() - failed to load library " + e.message();
			throw std::logic_error(msg);
		}
	}

	void Plugin::start()
	{
		switch (m_PluginState)
		{
			case Plugin::UNINSTALLED:
			case Plugin::INSTALLED:
			case Plugin::ACTIVE:
				break;

			case Plugin::LOADED:
				try
				{
					activate();
				}
				catch (std::logic_error e)
				{
					std::cout << "Plugin::start() - could not activate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void Plugin::activate()
	{
		try {
			
			if (m_PluginLoader.canCreate(m_PluginName))
			{
				m_PluginActivator = m_PluginLoader.create(m_PluginName);
				if (!m_PluginActivator)
				{
					throw std::logic_error("Plugin::activate() - failed to create plugin activator");
				}

				m_PluginActivator->start(m_PluginContextPtr);
				m_PluginState = Plugin::ACTIVE;
			}
			else
			{
				throw std::logic_error("Plugin::activate() - can't create plugin activator");
			}
		}
		catch (std::logic_error e)
		{
			if (m_PluginActivator)
			{
				delete m_PluginActivator;
			}
			
			throw e;
		}
	}

	void Plugin::stop()
	{
		switch (m_PluginState)
		{
			case Plugin::UNINSTALLED:
			case Plugin::INSTALLED:
			case Plugin::LOADED:
				break;

			case Plugin::ACTIVE:
				try
				{
					deactivate();
				}
				catch (std::logic_error e)
				{
					std::cout << "Plugin::stop() - could not deactivate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void Plugin::deactivate()
	{
		try
		{
			m_PluginActivator->stop(m_PluginContextPtr);
		
			try
			{
				m_PluginLoader.destroy(m_PluginName, m_PluginActivator);
			}
			catch (Poco::Exception& e)
			{
				std::string msg = "Plugin::deactivate() - failed to destroy plugin activator " + e.message();
				throw std::logic_error(msg);
			}
			
			delete m_PluginActivator;
			m_PluginActivator = NULL;
			m_PluginState = Plugin::LOADED;
		}
		catch (std::logic_error e)
		{
		}
	}

	void Plugin::unload()
	{
		switch (m_PluginState)
		{
			case Plugin::UNINSTALLED:
			case Plugin::INSTALLED:
			case Plugin::ACTIVE:
				break;

			case Plugin::LOADED:
				try
				{
					unloadLibrary();
				}
				catch (std::logic_error e)
				{
					std::cout << "Plugin::unload() - could not deactivate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void Plugin::unloadLibrary()
	{
		try
		{
			if(m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_PluginLoader.unloadLibrary(m_LibraryPath);
				m_PluginState = Plugin::INSTALLED;
			}
			else
			{
				throw std::logic_error("Plugin::unloadLibrary() - library was never loaded");
			}
		}
		catch(std::logic_error e)
		{
			throw e;
		}
	}
	
	void Plugin::uninstall()
	{
		switch (m_PluginState)
		{
		case Plugin::UNINSTALLED:
			break;

		case Plugin::ACTIVE:
			stop();
			m_PluginState = Plugin::LOADED;

		case Plugin::LOADED:
			unload();
			m_PluginState = Plugin::INSTALLED;

		case Plugin::INSTALLED:
			m_ContextPtr->uninstallPlugin(this);
			m_PluginState = Plugin::UNINSTALLED;
			break;
		}
	}
}