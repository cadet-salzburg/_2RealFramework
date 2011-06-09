#include "Plugin.h"
#include "PluginContext.h"
#include "PluginFrameworkContext.h"

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/DirectoryIterator.h"

#include <iostream>
#include <sstream>

namespace _2Real
{
	Plugin::Plugin(const std::string& name, PluginFrameworkContext* fwContext) :	m_FrameworkContext(fwContext), m_State(Plugin::UNINSTALLED), m_ActivationPolicy(Plugin::ACTIVATION_EAGER), m_Activator(NULL)
	{
		m_PluginName = name;
		m_LibraryPath  = std::string(Poco::Path::current() + "..\\..\\plugin\\" + m_PluginName);

		int sz = m_PluginName.size();
		if(m_PluginName[sz-1] == 'd' && m_PluginName[sz-2] == '_')
		{
			m_PluginName = m_PluginName.substr(0, sz-2);
		}

		m_Metadata.read(Poco::Path::current() + "..\\..\\plugin\\" + m_PluginName + ".xml");
		 
		m_Context = new PluginContext(this);
		m_State = Plugin::INSTALLED;
	}

	Plugin::~Plugin()
	{
		delete m_Context;
		delete m_Activator;
		m_FrameworkContext = NULL;
	}

	void Plugin::start(bool overrideStartingPolicy)
	{
		if (m_State == Plugin::UNINSTALLED)
		{
			std::cout << "Plugin::start() - plugin is already uninstalled" << std::endl;
			return;
		}
		else if (m_State == Plugin::ACTIVE)
		{
			//nothing to do here
			return;
		}
		else if (m_State == Plugin::INSTALLED)
		{
			//install all dependencies
			resolveDependencies();
			m_State = Plugin::RESOLVED;
		}

		if (!overrideStartingPolicy && m_ActivationPolicy == Plugin::ACTIVATION_LAZY)
		{
			if (m_State == Plugin::STARTING) 
			{
				return;
			}
			
			m_State = STARTING;
		}
		else
		{
			try
			{
				activate();
			}
			catch (std::logic_error e)
			{
				std::cout << "Plugin::start() - could not activate plugin " << e.what() << std::endl;
			}
		}
	}

	void Plugin::stop()
	{	
		switch (m_State)
		{
			//is already uninstalled
			case UNINSTALLED:
				std::cout << "Plugin::stop() - tried to stop uninstalled plugin" << std::endl;
				break;
			
			//was never started
			case INSTALLED:
			case RESOLVED:
				break;

			//was waiting to be activated
			//go back to resolved
			case STARTING:
				m_State = RESOLVED;
				break;

			//was active
			case ACTIVE:
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

	void Plugin::uninstall()
	{
		switch (m_State)
		{
		case UNINSTALLED:
			throw std::logic_error("Plugin::uninstall() - plugin is already uninstalled");
		
		case ACTIVE:
			deactivate();
			m_State = RESOLVED;
		
		case STARTING:
		case RESOLVED:
			
			//remove dependencies from framework
			removeDependencies();
			m_State = INSTALLED;

		case INSTALLED:
			
			//remove plugin from framework
			m_FrameworkContext->uninstallPlugin(this);
			m_State = UNINSTALLED;
		}
	}

	void Plugin::activate()
	{
		switch (m_State)
		{
		//this should not actually happen, ever
		case Plugin::INSTALLED:
			throw std::logic_error("Plugin::activate() - plugin was never resolved");
		case Plugin::ACTIVE:
			throw std::logic_error("Plugin::activate() - plugin is already active");
		case Plugin::UNINSTALLED:
			throw std::logic_error("Plugin::activate() - plugin is already uninstalled");
		
		//can be activated
		case Plugin::STARTING:
		case Plugin::RESOLVED:
			try
			{
				//start all the plugin on which this one depends first
				startDependencies();
				loadLibrary();
			}
			catch (std::logic_error e)
			{
				stopDependencies();
				//go back to resolved state
				m_State = Plugin::RESOLVED;
				throw std::logic_error("Plugin::activate could not load library");
			}
			break;
		}
	}

	void Plugin::deactivate()
	{
		switch (m_State)
		{
		//these should not happen
		case Plugin::INSTALLED:
			throw std::logic_error("Plugin::deactivate() - plugin is in installed state");
		case Plugin::RESOLVED:
			throw std::logic_error("Plugin::deactivate() - plugin is in resolved state");
		case Plugin::UNINSTALLED:
			throw std::logic_error("Plugin::deactivate() - plugin is in uninstalled state");
		case Plugin::STARTING:
			throw std::logic_error("Plugin::deactivate() - plugin is in starting state");
		
		case Plugin::ACTIVE:
			try
			{
				unloadLibrary();
			}
			catch (std::logic_error e)
			{
				std::cout << "Plugin::deactivate() - could not unload library " << e.what() << std::endl;
			}

			stopDependencies();
			//go back to resolved state
			m_State = Plugin::RESOLVED;
			break;
		}
	}

	void Plugin::loadLibrary()
	{
		try {
			
			m_PluginLoader.loadLibrary(m_LibraryPath);
			if (!m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				throw std::logic_error("Plugin::loadLibrary() - could not load library");
			}

			if(m_PluginLoader.canCreate(m_PluginName))
			{
				m_Activator = m_PluginLoader.create(m_PluginName);
				if (!m_Activator)
				{
					throw std::logic_error("Plugin::loadLibrary() - could create plugin activator");
				}

				m_Activator->start(m_Context);
				m_State = Plugin::ACTIVE;
			}
			else
			{
				throw std::logic_error("Plugin::loadLibrary() - cannot create plugin activator");
			}
		}
		catch(std::logic_error e)
		{
			if (!m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_PluginLoader.unloadLibrary(m_LibraryPath);
			}

			throw e;
		}
	}

	void Plugin::unloadLibrary()
	{
		try
		{
			if(m_PluginLoader.isLibraryLoaded(m_LibraryPath))
			{
				m_Activator->stop(m_Context);
				m_PluginLoader.unloadLibrary(m_LibraryPath);
				
				delete [] m_Activator;
				m_Activator = NULL;
			}
			else
			{
				throw std::logic_error("Plugin::loadLibrary() - library was never loaded");
			}
		}
		catch(std::logic_error e)
		{
			throw e;
		}
	}

	void Plugin::resolveDependencies()
	{
		/*
		const std::string dependencies  = m_Metadata.getAttribute("dependencies");
		std::istringstream pluginstream(dependencies);
		std::vector<std::string> plugins;
		plugins.clear();

		std::string plugin;
		while(std::getline(pluginstream, plugin, ' '))
		{
			plugins.push_back(plugin);
		}

		for (std::vector<std::string>::iterator it = plugins.begin(); it != plugins.end(); it++)
		{
			Plugin* p = m_FrameworkContext->installPlugin(*it);
			m_Dependencies.insert(std::make_pair<std::string, Plugin*>(*it, p));
		}
		*/
	}

	void Plugin::startDependencies()
	{
		/*
		for (std::map<std::string, Plugin*>::iterator it = m_Dependencies.begin(); it != m_Dependencies.end(); it++)
		{
			it->second->start(true);
		}
		*/
	}

	void Plugin::removeDependencies()
	{
		/*
		for (std::map<std::string, Plugin*>::iterator it = m_Dependencies.begin(); it != m_Dependencies.end(); it++)
		{
			m_FrameworkContext->uninstallPlugin(it->second);
		}
		*/
	}

	void Plugin::stopDependencies()
	{
		/*
		for (std::map<std::string, Plugin*>::iterator it = m_Dependencies.begin(); it != m_Dependencies.end(); it++)
		{
			it->second->stop();
		}
		*/
	}

	PluginFrameworkContext* Plugin::getFrameworkContext()
	{
		return m_FrameworkContext;
	}

	PluginContext* Plugin::getContext()
	{
		return m_Context;
	}
}