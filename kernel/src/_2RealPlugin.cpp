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

namespace _2Real
{
	_2RealPlugin::_2RealPlugin(const std::string& _name, const std::string& _path, _2RealContextPrivate* _context) : 
						m_PluginState(_2RealPlugin::UNINSTALLED), m_PluginName(_name), m_LibraryPath(_path), m_ContextPtr(_context), m_PluginActivator(NULL)
	{
		m_PluginContextPtr = _2RealPluginContextPtr(new _2RealPluginContext(this, _context));
		m_PluginState = _2RealPlugin::INSTALLED;
	}

	_2RealPlugin::~_2RealPlugin()
	{
		delete m_PluginContextPtr.assign(NULL);
	}

	const _2RealPlugin::_2RealPluginState _2RealPlugin::state() const
	{
		return m_PluginState;
	}
		
	const std::string& _2RealPlugin::name() const
	{
		return m_PluginName;
	}

	const std::string& _2RealPlugin::path() const
	{
		return m_LibraryPath;
	}

	void _2RealPlugin::load()
	{
		switch (m_PluginState)
		{
			case _2RealPlugin::UNINSTALLED:
			case _2RealPlugin::LOADED:
			case _2RealPlugin::ACTIVE:
				break;

			case _2RealPlugin::INSTALLED:
				try
				{
					loadLibrary();
				}
				catch (std::logic_error e)
				{
					std::cout << "_2RealPlugin::load() - could not load the dll " << e.what() << std::endl;
				}
				break;
		}
	}

	void _2RealPlugin::loadLibrary()
	{
		try
		{
			m_PluginLoader.loadLibrary(m_LibraryPath);
			m_PluginState = _2RealPlugin::LOADED;
		}
		catch (Poco::Exception& e)
		{
			std::string msg = "Plugin::loadLibrary() - failed to load library " + e.message();
			throw std::logic_error(msg);
		}
	}

	void _2RealPlugin::start()
	{
		switch (m_PluginState)
		{
			case _2RealPlugin::UNINSTALLED:
			case _2RealPlugin::INSTALLED:
			case _2RealPlugin::ACTIVE:
				break;

			case _2RealPlugin::LOADED:
				try
				{
					activate();
				}
				catch (std::logic_error e)
				{
					std::cout << "_2RealPlugin::start() - could not activate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void _2RealPlugin::activate()
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
				m_PluginState = _2RealPlugin::ACTIVE;
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

	void _2RealPlugin::stop()
	{
		switch (m_PluginState)
		{
			case _2RealPlugin::UNINSTALLED:
			case _2RealPlugin::INSTALLED:
			case _2RealPlugin::LOADED:
				break;

			case _2RealPlugin::ACTIVE:
				try
				{
					deactivate();
				}
				catch (std::logic_error e)
				{
					std::cout << "_2RealPlugin::stop() - could not deactivate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void _2RealPlugin::deactivate()
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
			m_PluginState = _2RealPlugin::LOADED;
		}
		catch (std::logic_error e)
		{
		}
	}

	void _2RealPlugin::unload()
	{
		switch (m_PluginState)
		{
			case _2RealPlugin::UNINSTALLED:
			case _2RealPlugin::INSTALLED:
			case _2RealPlugin::ACTIVE:
				break;

			case _2RealPlugin::LOADED:
				try
				{
					unloadLibrary();
				}
				catch (std::logic_error e)
				{
					std::cout << "_2RealPlugin::unload() - could not deactivate plugin " << e.what() << std::endl;
				}
				break;
		}
	}

	void _2RealPlugin::unloadLibrary()
	{
		try
		{
			if(m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_PluginLoader.unloadLibrary(m_LibraryPath);
				m_PluginState = _2RealPlugin::INSTALLED;
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
	
	void _2RealPlugin::uninstall()
	{
		switch (m_PluginState)
		{
		case _2RealPlugin::UNINSTALLED:
			break;

		case _2RealPlugin::ACTIVE:
			stop();
			m_PluginState = _2RealPlugin::LOADED;

		case _2RealPlugin::LOADED:
			unload();
			m_PluginState = _2RealPlugin::INSTALLED;

		case _2RealPlugin::INSTALLED:
			m_ContextPtr->uninstallPlugin(this);
			m_PluginState = _2RealPlugin::UNINSTALLED;
			break;
		}
	}
}